// Fill out your copyright notice in the Description page of Project Settings.


#include "Hall/Table.h"
#include "Components/BoxComponent.h"

#include "Hall/Chair.h"
#include "Bakery/BakeryDefines.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Kitchen/Ingredient.h"
#include "Characters/Customer.h"

ATable::ATable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCanEverAffectNavigation(false);
}

void ATable::BeginPlay()
{
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

	for (AChair* Seat : UsingSeats)
	{
		ACustomer* Customer = Seat->GetAssignedCustomer();
		if (Customer->GetOrder() == Ingredient->GetAvailableRecipe())
		{
			Grabber->Release();

			// TODO: 좌석 위치에 따라 접시를 테이블 위에 배치

			Customer->ServeDish(Ingredient);
		}
	}
}
