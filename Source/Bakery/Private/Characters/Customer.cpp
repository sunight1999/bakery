// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Customer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"

#include "General/BakeryGameMode.h"
#include "General/BakeryGameState.h"
#include "Subsystems/SoundManager.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/Dish.h"
#include "Kitchen/Data/RecipeData.h"
#include "Hall/Table.h"
#include "Hall/Chair.h"
#include "Characters/PlayerPawn.h"
#include "Widgets/Customer/OrderWaitingTimeBarWidget.h"

/*
 * 손님 행동 순서
 * Idle -> Sitting -> Ordering -> WaitingDish -> Eating -> Standing -> Leaving
 *    1. Idle: Spawn 후 HallManager로부터 자리를 할당받아 해당 위치로 이동. 자리가 없을 경우 지정 위치에서 대기 | HandleIdle()
 *	  2. Sitting: 지정 자리로 이동이 끝나면 앉기 애니메이션 수행 | 앉기 애니메이션 종료 시 RequestTakeOrder()
 *	  3. Ordering: 주문 접수 대기 | Table에 '상호작용' 시 TakeOrder()
 *	  4. WaitingDish: 음식 서빙 대기 | Table에 음식 '놓기' 시 ServeDish() -> Eat()
 *	  5. Eating: EatingTime만큼 식사 | FinishEating()
 *	  6. Standing: 일어서기 애니메이션 수행 | 일어서기 애니메이션 종료 시 Leave()
 *	  7. Leaving: 식사를 끝내고 카메라 바깥으로 이동, Despawn 수행
 */

ACustomer::ACustomer()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 250.f;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Customer"));
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACustomer::OnCusotmerHit);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DishWaitingTimeBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DishWaitingTimeBarWidget"));
	DishWaitingTimeBarWidget->SetVisibility(false);
	DishWaitingTimeBarWidget->SetupAttachment(RootComponent);
}

void ACustomer::BeginPlay()
{
	Super::BeginPlay();

	CustomerController = CastChecked<AAIController>(GetController());

	TimerManager = &GetWorldTimerManager();
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CustomerState == ECustomerState::Idle)
	{
		HandleIdle();
	}
	else if (CustomerState == ECustomerState::Sitting)
	{
		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), SitTargetTransform.GetLocation(), DeltaTime, SitMovementSpeed);
		FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), SitTargetTransform.Rotator(), DeltaTime, SitRotationSpeed);
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
	else if (CustomerState == ECustomerState::WaitingDish)
	{
		float CurrentTime = GetRemainingWaitingTime();
		float WaitingTime = GetWaitingTime();
		DishWaitingTimeBar->SetPercentage(CurrentTime / WaitingTime);
	}
	else if (CustomerState == ECustomerState::Standing)
	{
		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MoveTargetPosition, DeltaTime, SitMovementSpeed);
		SetActorLocation(NewLocation);
	}
}

void ACustomer::OnCusotmerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA<APlayerPawn>())
	{
		FVector PushDirection = OtherActor->GetActorLocation() - GetActorLocation();
		PushDirection.Normalize();
		OtherActor->AddActorWorldOffset(PushDirection * PlayerPushForce);
	}
}

void ACustomer::SetOrder(const URecipeData* RecipeData)
{
	Order = RecipeData;

	if (!DishWaitingTimeBar)
	{
		DishWaitingTimeBar = Cast<UDishWaitingTimeBarWidget>(DishWaitingTimeBarWidget->GetUserWidgetObject());
	}

	FString OrderName = Order->GetName().ToString();
	FString OrderImagePath = "/Game/Textures/Orders/" + OrderName + "." + OrderName;
	DishWaitingTimeBar->SetOrderImage(OrderImagePath);
}

/*
 * 상태별 처리 함수 (외부 액터에 의해 호출 또는 상태 변경)
 */
void ACustomer::RequestTakeOrder()
{
	CustomerState = ECustomerState::Ordering;

	AssignedSeat->GetOwnerTable()->RequestTakeOrder(this);
	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("Order"), GetActorLocation());

	SetWaitingTimer(OrderWaitingTime);
}

void ACustomer::TakeOrder()
{
	OrderDish();
}

bool ACustomer::ServeDish(ADish* Dish)
{
	if (CustomerState != ECustomerState::WaitingDish)
	{
		return false;
	}

	if (!Dish || !Dish->PeekDessert())
	{
		return false;
	}

	if (Dish->PeekDessert()->GetAvailableRecipe() != Order)
	{
		return false;
	}

	Eat(Dish);

	return true;
}

void ACustomer::Disappoint()
{
	ClearWaitingTimer();

	Leave();

	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	BakeryGameMode->Disappointed(this);
}

void ACustomer::Leave()
{
	// 가게 영업이 끝나면 모든 손님에 대해 Leave()를 호출하므로 이미 떠나는 중인지 여부 확인
	if (CustomerState == ECustomerState::Leaving)
	{
		return;
	}

	CustomerState = ECustomerState::Leaving;

	// 타이머 및 UI 정리 (식사 완료 전 영업이 종료되는 경우를 위해)
	ClearWaitingTimer();
	TimerManager->ClearTimer(EatingTimer);
	DishWaitingTimeBarWidget->SetVisibility(false);

	// 테이블 정리
	if (AssignedSeat)
	{
		ATable* AssignedTable = AssignedSeat->GetOwnerTable();
		AssignedTable->LeaveSeat(AssignedSeat);

		AssignedSeat->PutInChair();
		AssignedSeat = nullptr;
	}

	if (ServedDish)
	{
		ServedDish = nullptr;
	}

	MoveToDespawn();
}

/*
 * 상태별 처리 함수
 */
void ACustomer::HandleIdle()
{
	if (AssignedSeat && IsInTargetPosition(AcceptanceRadius))
	{
		CustomerState = ECustomerState::Sitting;
		
		CustomerController->UnPossess();

		// GetSitPoint()를 호출하는 시점이 의자를 빼기 전이므로 뺐을 때의 위치를 계산해 설정
		FVector SitLocation = AssignedSeat->GetSitPointLocation();
		FQuat SitRotation = AssignedSeat->GetSitPointRotation().Quaternion();
		SitLocation += AssignedSeat->GetActorForwardVector() * -AssignedSeat->GetPullOutDistance();
		SitLocation.Z = GetActorLocation().Z + AssignedSeat->GetSitPointRelativeLocation().Z;
		SitTargetTransform.SetLocation(SitLocation);
		SitTargetTransform.SetRotation(SitRotation);

		AssignedSeat->PullOutChair();

		// Sitting 애니메이션 종료 후 애니메이션 블루프린트에서 RequestTakeOrder() 호출
	}
}

void ACustomer::OrderDish()
{
	CustomerState = ECustomerState::WaitingDish;

	DishWaitingTimeBarWidget->SetVisibility(true);

	SetWaitingTimer(DishWaitingTime);
}

void ACustomer::Eat(ADish* Dish)
{
	CustomerState = ECustomerState::Eating;

	ServedDish = Dish;
	DishWaitingTimeBarWidget->SetVisibility(false);

	ClearWaitingTimer();
	TimerManager->SetTimer(EatingTimer, this, &ACustomer::FinishEating, EatingTime, false, EatingTime);
}

void ACustomer::FinishEating()
{
	CustomerState = ECustomerState::Standing;

	ServedDish->Eat();
	ServedDish = nullptr;

	// 먹은 음식 계산
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	BakeryGameState->AddMoney(Order->GetPrice());

	// Standing 애니메이션 종료 후 애니메이션 블루프린트에서 Leave() 호출
}

/*
 * 주문 처리 관련 함수
 */
FORCEINLINE void ACustomer::SetWaitingTimer(float WaitingTime)
{
	ClearWaitingTimer();
	TimerManager->SetTimer(WaitingTimer, this, &ACustomer::Disappoint, WaitingTime, false, WaitingTime);
}

FORCEINLINE void ACustomer::ClearWaitingTimer()
{
	TimerManager->ClearTimer(WaitingTimer);
}

/*
 * 이동 관련 함수
 */
void ACustomer::MoveTo(const FVector& InTargetPosition)
{
	if (!CustomerController->GetPawn())
	{
		CustomerController->Possess(this);
	}

	MoveTargetPosition = InTargetPosition;
	CustomerController->MoveToLocation(MoveTargetPosition, AcceptanceRadius, false);
}

void ACustomer::MoveToDespawn()
{
	MoveTo(DespawnPosition);
}

void ACustomer::SitTo(AChair* Seat)
{
	AssignedSeat = Seat;
	AssignedSeat->SetAssignedCustomer(this);

	FVector NewLocation = AssignedSeat->GetStopToSitPointLocation();
	NewLocation.Z = GetActorLocation().Z;
	MoveTo(NewLocation);
}

bool ACustomer::IsInTargetPosition(const float ToleranceRadius) const
{
	float DistanceSquared = FVector::DistSquared2D(GetActorLocation(), MoveTargetPosition);
	return DistanceSquared <= ToleranceRadius * ToleranceRadius;
}
