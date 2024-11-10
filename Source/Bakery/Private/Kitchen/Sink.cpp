// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Sink.h"
#include "Components/BoxComponent.h"

#include "Kitchen/Dish.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"

ASink::ASink()
{
	BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
	BottomMesh->SetupAttachment(InteractionBox);

	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(BottomMesh);

	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
	WaterMesh->SetupAttachment(TopMesh);

	DishKeepingBeginPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DishKeepingBeginPoint"));
	DishKeepingBeginPoint->SetupAttachment(WaterMesh);
}

void ASink::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	for (ADish* Dish : KeepingDishes)
	{
		Dish->Wash();
	}
}

void ASink::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	if (Grabbed)
	{
		ADish* Dish = Cast<ADish>(Grabbed->GetOwner());
		if (!Dish || Dish->IsClean() || KeepingDishes.Num() >= MaxDishNum)
		{
			return;
		}

		KeepingDishes.Emplace(Dish);
		Dish->SetActorLocation(CalculateDishLocation(KeepingDishes.Num() - 1));

		UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
		if (Primitive)
		{
			Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
			Primitive->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		}

		Grabber->Release();
	}
	else
	{
		bool bIsAvailable = false;
		int GrabbedDishIndex = 0;

		// 깨끗한 접시가 존재할 경우 플레이어가 잡도록 설정
		for (int i = KeepingDishes.Num() - 1; i >= 0; i--)
		{
			ADish* Dish = KeepingDishes[i];
			if (Dish->IsClean())
			{
				bIsAvailable = true;
				GrabbedDishIndex = i;

				UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
				Grabber->Grab(Primitive, Dish->GetActorLocation());

				KeepingDishes.RemoveAt(i);
			}
		}

		if (!bIsAvailable)
		{
			return;
		}


		// 빈 공간 채우기
		for (int i = GrabbedDishIndex; i < KeepingDishes.Num(); i++)
		{
			ADish* Dish = KeepingDishes[i];
			Dish->SetActorLocation(CalculateDishLocation(i));
		}
	}
}

FVector ASink::CalculateDishLocation(int Index)
{
	FVector Begin = DishKeepingBeginPoint->GetComponentLocation();
	FVector Offset = DishKeepingBeginPoint->GetRightVector() * (bKeepingToTheRight ? -KeepingDishDistance : KeepingDishDistance);
	
	return Begin + Offset * Index;
}
