// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Customer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"

#include "Interactions/Interactables/InteractableComponent.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/Data/RecipeData.h"
#include "Bakery/HallManager.h"
#include "Hall/Table.h"
#include "Hall/Chair.h"

ACustomer::ACustomer()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 250.f;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Customer"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Interactable = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	// TODO: Interactable Component 내부에서 (IInteract를 구현하는)Owner Actor의 함수 알아서 바인딩하기
	Interactable->OnEnterInteractDelegate.AddDynamic(this, &ACustomer::OnEnterInteract);
}

void ACustomer::BeginPlay()
{
	Super::BeginPlay();

	CustomerController = CastChecked<AAIController>(GetController());
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CustomerState)
	{
	case ECustomerState::Idle:
		HandleIdle();
		break;

	case ECustomerState::Sitting:
		HandleSitting();
		break;

	case ECustomerState::Ordering:
		HandleOrdering();
		break;

	case ECustomerState::WaitingDish:
		HandleWaitingDish();
		break;

	case ECustomerState::Eating:
		HandleEating();
		break;

	default:
		break;
	}
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
		
		// TODO: 앉기 애니메이션 끝난 후 주문 요청 아이콘 띄우기
		CustomerState = ECustomerState::Ordering;
	}
}

void ACustomer::HandleSitting()
{
}

void ACustomer::HandleOrdering()
{
	// TODO: 주문 대기 시간 측정
	UE_LOG(LogTemp, Display, TEXT("%s 줘"), *Order->GetName().ToString());
}

void ACustomer::HandleWaitingDish()
{
	// TODO: 음식 대기 시간 측정
}

void ACustomer::HandleEating()
{
	// TODO: 식사 시간 측정 후 AssignedTable->LeaveSeat 호출

	ATable* AssignedTable = AssignedSeat->GetOwnerTable();
	AssignedTable->LeaveSeat(AssignedSeat);

	AssignedSeat = nullptr;
	CustomerState = ECustomerState::Idle;

	// TODO: CustomSpawner가 Spawn시 파괴 지점도 지정하도록 수정
	MoveTo(FVector(180, -860, 0));
}

/*
 * 주문 처리 관련 함수
 */
bool ACustomer::ServeDish(AIngredient* Dish)
{
	if (Dish->GetAvailableRecipe() != Order)
	{
		return false;
	}

	CustomerState = ECustomerState::Eating;
	Dish->Destroy();

	UE_LOG(LogTemp, Display, TEXT("냠냠"));

	return true;
}

/*
 * 이동 관련 함수
 */
void ACustomer::MoveTo(const FVector& InTargetPosition)
{
	TargetPosition = InTargetPosition;
	CustomerController->MoveToLocation(TargetPosition);
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

/*
 * Interaction Functions
 */
void ACustomer::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	if (CustomerState != ECustomerState::Ordering)
	{
		return;
	}

	// TODO: 주문 아이콘으로 띄우기

	CustomerState = ECustomerState::WaitingDish;
}
