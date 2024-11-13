// Fill out your copyright notice in the Description page of Project Settings.


#include "Hall/Table.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Hall/Chair.h"
#include "General/BakeryDefines.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "General/BakeryGameMode.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/Dish.h"
#include "Characters/Customer.h"
#include "Widgets/Hall/WaitingTimeBarWidget.h"

ATable::ATable()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCanEverAffectNavigation(false);

	DishServingCenterPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DishServingCenterPoint"));
	DishServingCenterPoint->SetupAttachment(StaticMesh);

	WaitingTimeBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WaitingTimeBarWidget"));
	WaitingTimeBarWidget->SetVisibility(false);
	WaitingTimeBarWidget->SetupAttachment(RootComponent);
}

void ATable::BeginPlay()
{
	Super::BeginPlay();

	BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());

	// 테이블 주변으로 의자 생성
	// TODO: 테이블이 가까이 있으면 의자가 하나만 생성되도록 해야 함
	for (int i = 0; i < 4; i++)
	{
		AChair* Chair = Cast<AChair>(GetWorld()->SpawnActor(ChairClass));
		FVector Direction = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;

		switch (i)
		{
		case (uint8)EDirection::Left:
			Chair->SetChairDirection(FVector::LeftVector);
			Rotation.Yaw = 90.f;
			break;

		case (uint8)EDirection::Right:
			Chair->SetChairDirection(FVector::RightVector);
			Rotation.Yaw = 270.f;
			break;

		case (uint8)EDirection::Forward:
			Chair->SetChairDirection(FVector::ForwardVector);
			Rotation.Yaw = 180.f;
			break;

		case (uint8)EDirection::Backward:
			Chair->SetChairDirection(FVector::BackwardVector);
			break;
		}

		Chair->SetOwnerTable(this);
		Chair->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Chair->SetActorLocationAndRotation(GetActorLocation() + Chair->GetChairDirection() * ChairDistance, Rotation);
		Chair->Initialize();
		EmptySeats.Emplace(Chair);
	}
}

void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaitingTrackedCustomer)
	{
		float CurrentTime = WaitingTrackedCustomer->GetRemainingWaitingTime();
		float WaitingTime = WaitingTrackedCustomer->GetWaitingTime();
		WaitingTimeBar->SetPercentage(CurrentTime / WaitingTime);
	}
}

AChair* ATable::RequestSeat()
{
	if (EmptySeats.Num() <= 0)
	{
		return nullptr;
	}

	AChair* SelectedSeat = EmptySeats.Pop();
	UsingSeats.Add(SelectedSeat);

	return SelectedSeat;
}

void ATable::LeaveSeat(AChair* Seat)
{
	WaitingTrackedCustomer = nullptr;
	Seat->SetAssignedCustomer(nullptr);

	UsingSeats.Remove(Seat);
	EmptySeats.Emplace(Seat);

	// 영업 종료 시 주문 접수를 못한 채 떠나는 손님이 있을 수 있으므로 UI 해제 추가 확인
	if (UsingSeats.Num() == 0)
	{
		WaitingTimeBarWidget->SetVisibility(false);
	}
}

void ATable::RequestTakeOrder(ACustomer* Customer)
{
	if (WaitingTrackedCustomer)
	{
		return;
	}

	if (!WaitingTimeBar)
	{
		WaitingTimeBar = Cast<UWaitingTimeBarWidget>(WaitingTimeBarWidget->GetUserWidgetObject());
	}
	
	WaitingTimeBar->Reset();
	WaitingTimeBarWidget->SetVisibility(true);
	WaitingTrackedCustomer = Customer;
}


void ATable::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	if (!WaitingTrackedCustomer)
	{
		return;
	}

	for (AChair* Seat : UsingSeats)
	{
		ACustomer* Customer = Seat->GetAssignedCustomer();
		if (Customer->GetState() == ECustomerState::Ordering)
		{
			Customer->TakeOrder();
		}
	}

	WaitingTrackedCustomer = nullptr;
	WaitingTimeBarWidget->SetVisibility(false);
}

void ATable::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	// 플레이어가 빈 손이면 테이블에 빈 접시가 있는지 확인 후 Grab 설정
	if (!Grabbed)
	{
		for (int i = 0; i < ServedDishes.Num(); i++)
		{
			ADish* Dish = ServedDishes[i];
			if (!Dish->PeekDessert() || !BakeryGameMode->IsOpened())
			{
				UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
				Grabber->Grab(Primitive, Dish->GetActorLocation());

				ServedDishes.RemoveAt(i);
				break;
			}
		}

		return;
	}

	ADish* Dish = Cast<ADish>(Grabbed->GetOwner());
	if (!Dish)
	{
		return;
	}

	// 해당 요리를 주문한 손님이 있는지 확인 후 제공
	for (AChair* Seat : UsingSeats)
	{
		ACustomer* Customer = Seat->GetAssignedCustomer();
		if (Customer->GetState() != ECustomerState::WaitingDish)
		{
			continue;
		}

		if (Customer->ServeDish(Dish))
		{
			Grabber->Release();

			FVector TableCenterLocation = DishServingCenterPoint->GetComponentLocation();
			FVector DishLocation = TableCenterLocation + Seat->GetChairDirection() * DishesDistance;
			Dish->SetActorLocation(DishLocation);

			UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
			Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
			Primitive->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

			ServedDishes.Emplace(Dish);
			return;
		}
	}
}
