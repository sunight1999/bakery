// Fill out your copyright notice in the Description page of Project Settings.


#include "Hall/Table.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"

#include "Hall/Chair.h"
#include "General/BakeryDefines.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "General/BakeryGameMode.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/Dish.h"
#include "Characters/Customer.h"

ATable::ATable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCanEverAffectNavigation(false);

	DishServingCenterPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DishServingCenterPoint"));
	DishServingCenterPoint->SetupAttachment(StaticMesh);
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
	Seat->SetAssignedCustomer(nullptr);

	UsingSeats.Remove(Seat);
	EmptySeats.Emplace(Seat);
}

void ATable::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	for (AChair* Seat : UsingSeats)
	{
		ACustomer* Customer = Seat->GetAssignedCustomer();
		if (Customer->GetState() == ECustomerState::Ordering)
		{
			Customer->TakeOrder();
		}
	}
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

	if (Dish->IsLostDish())
	{
		for (AChair* Seat : UsingSeats)
		{
			ACustomer* Customer = Seat->GetAssignedCustomer();
			if (!Customer->IsFeared() || !Customer->IsDishBelongs(Dish))
			{
				continue;
			}

			Grabber->Release();
			PutDish(Dish, Seat);

			Customer->SetFeared(false);

			break;
		}

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
			PutDish(Dish, Seat);

			return;
		}
	}
}

void ATable::PutDish(ADish* Dish, AChair* Chair)
{
	FVector TableCenterLocation = DishServingCenterPoint->GetComponentLocation();
	FVector DishLocation = TableCenterLocation + Chair->GetChairDirection() * DishesDistance;
	Dish->SetActorLocation(DishLocation);
	
	UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
	Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Primitive->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	Primitive->SetSimulatePhysics(false);

	ServedDishes.Emplace(Dish);
	return;
}