// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Customer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"

#include "General/BakeryGameMode.h"
#include "General/BakeryGameState.h"
#include "Subsystems/SoundManager.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/Data/RecipeData.h"
#include "Hall/Table.h"
#include "Hall/Chair.h"
#include "Widgets/Customer/OrderWaitingTimeBarWidget.h"

/*
 * 손님 행동 순서
 * Idle -> Sitting -> Ordering -> WaitingDish -> Eating -> Idle
 *    1. Idle: Spawn 후 HallManager로부터 자리를 할당받아 해당 위치로 이동. 자리가 없을 경우 지정 위치에서 대기 -> SitTo()
 *	  2. Sitting: 지정 자리로 이동이 끝나면 앉기 애니메이션 수행 -> HandleIdle()
 *	  3. Ordering: 주문 접수 대기 -> Table에 상호작용 시 TakeOrder()
 *	  4. WaitingDish: 음식 서빙 대기 -> Table에 음식 놓기 시 ServeDish() -> Eat()
 *	  5. Eating: EatingTime만큼 식사 -> FinishEating()
 *	  6. Idle: 식사를 끝내고 카메라 바깥으로 이동, Destroy 또는 Disable 수행
 */

ACustomer::ACustomer()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 250.f;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Customer"));

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
		HandleSitting();
	}
	else if (CustomerState == ECustomerState::WaitingDish)
	{
		float CurrentTime = GetRemainingWaitingTime();
		float WaitingTime = GetWaitingTime();
		DishWaitingTimeBar->SetPercentage(CurrentTime / WaitingTime);
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

void ACustomer::TakeOrder()
{
	OrderDish();
}

/*
 * 상태별 처리 함수
 */
void ACustomer::HandleIdle()
{
	if (AssignedSeat && IsInTargetPosition())
	{
		// TODO: 앉기 애니메이션 시작
		CustomerState = ECustomerState::Sitting;

		RequestTakeOrder();
	}
}

void ACustomer::HandleSitting()
{
}

void ACustomer::RequestTakeOrder()
{
	CustomerState = ECustomerState::Ordering;

	AssignedSeat->GetOwnerTable()->RequestTakeOrder(this);
	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("Order"), GetActorLocation());

	SetWaitingTimer(OrderWaitingTime);
}

void ACustomer::OrderDish()
{
	CustomerState = ECustomerState::WaitingDish;

	DishWaitingTimeBarWidget->SetVisibility(true);

	SetWaitingTimer(DishWaitingTime);
}

void ACustomer::Eat(AIngredient* Dish)
{
	ServedDish = Dish;
	CustomerState = ECustomerState::Eating;

	DishWaitingTimeBarWidget->SetVisibility(false);

	ClearWaitingTimer();
	TimerManager->SetTimer(EatingTimer, this, &ACustomer::FinishEating, EatingTime, false, EatingTime);
}

void ACustomer::FinishEating()
{
	ServedDish->Destroy();
	ServedDish = nullptr;

	// 먹은 음식 계산
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	BakeryGameState->AddMoney(Order->GetPrice());

	Leave();
}

void ACustomer::Leave()
{
	// 타이머 및 UI 정리 (식사 완료 전 영업이 종료되는 경우를 위해)
	ClearWaitingTimer();
	TimerManager->ClearTimer(EatingTimer);
	DishWaitingTimeBarWidget->SetVisibility(false);

	// 테이블 정리
	if (AssignedSeat)
	{
		ATable* AssignedTable = AssignedSeat->GetOwnerTable();
		AssignedTable->LeaveSeat(AssignedSeat);
		AssignedSeat = nullptr;
	}

	// TODO: 영업 종료 시 이미 서빙된 음식 어떻게 할 건지 기획 필요
	if (ServedDish)
	{
		ServedDish->Destroy();
		ServedDish = nullptr;
	}

	CustomerState = ECustomerState::Idle;

	MoveTo(DespawnPosition);
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

bool ACustomer::ServeDish(AIngredient* Dish)
{
	if (CustomerState != ECustomerState::WaitingDish)
	{
		return false;
	}
	
	if (Dish->GetAvailableRecipe() != Order)
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

/*
 * 이동 관련 함수
 */
void ACustomer::MoveTo(const FVector& InTargetPosition)
{
	TargetPosition = InTargetPosition;
	CustomerController->MoveToLocation(TargetPosition, 10.f);
}

void ACustomer::SitTo(AChair* Seat)
{
	AssignedSeat = Seat;
	AssignedSeat->SetAssignedCustomer(this);

	MoveTo(Seat->GetActorLocation());
}

bool ACustomer::IsInTargetPosition(const float ToleranceRadius) const
{
	float DistanceSquared = FVector::DistSquared2D(GetActorLocation(), TargetPosition);
	return DistanceSquared <= ToleranceRadius * ToleranceRadius;
}
