// Fill out your copyright notice in the Description page of Project Settings.


#include "Hall/Table.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Hall/Chair.h"
#include "Bakery/BakeryDefines.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Kitchen/Ingredient.h"
#include "Characters/Customer.h"
#include "Widgets/Hall/WaitingTimeBarWidget.h"

ATable::ATable()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCanEverAffectNavigation(false);

	WaitingTimeBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WaitingTimeBarWidget"));
	WaitingTimeBarWidget->SetVisibility(false);
	WaitingTimeBarWidget->SetupAttachment(RootComponent);
}

void ATable::BeginPlay()
{
	Super::BeginPlay();

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
			Direction = FVector::LeftVector;
			break;

		case (uint8)EDirection::Right:
			Direction = FVector::RightVector;
			Rotation.Yaw = 180.f;
			break;

		case (uint8)EDirection::Forward:
			Direction = FVector::ForwardVector;
			Rotation.Yaw = 90.f;
			break;

		case (uint8)EDirection::Backward:
			Direction = FVector::BackwardVector;
			Rotation.Yaw = 270.f;
			break;
		}

		Chair->SetOwnerTable(this);
		Chair->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Chair->SetActorLocationAndRotation(GetActorLocation() + Direction * ChairDistance, Rotation);

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
	// 플레이어의 Grabbed 가져와서 주변 손님의 레시피와 확인
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();
	if (!Grabbed)
	{
		return;
	}

	AIngredient* Ingredient = Cast<AIngredient>(Grabbed->GetOwner());
	if (!Ingredient)
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

		if (Customer->ServeDish(Ingredient))
		{
			Grabber->Release();

			// TODO: 좌석 위치에 따라 접시를 테이블 위에 배치

			return;
		}
	}
}
