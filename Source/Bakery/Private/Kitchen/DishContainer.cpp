// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/DishContainer.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Kitchen/Dish.h"

ADishContainer::ADishContainer()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADishContainer::End()
{
	DishPool.End();
}

void ADishContainer::BeginPlay()
{
	Super::BeginPlay();

	DishPool.OnPostSpawned.BindUObject(this, &ADishContainer::InitializeDish);
	DishPool.Initialize(GetWorld(), DishClass, this);
}

void ADishContainer::InitializeDish(UObject* DishObj)
{
	AActor* Dish = Cast<AActor>(DishObj);
	if (!Dish)
	{
		return;
	}

	Dish->SetActorLocationAndRotation(GetActorLocation(), FRotator::ZeroRotator);

	UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
	if (!Primitive)
	{
		return;
	}

	Primitive->SetConstraintMode(EDOFMode::XYPlane);
}

bool ADishContainer::TryReturnDish(UObject* DishObj)
{
	ADish* Dish = Cast<ADish>(DishObj);
	if (!Dish || !Dish->IsClean())
	{
		return false;
	}

	return DishPool.Put(DishObj);
}

void ADishContainer::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	// 이미 플레이어가 쥐고 있는 물건이 있을 경우 return
	if (Grabbed)
	{
		ADish* Dish = Cast<ADish>(Grabbed->GetOwner());
		if (!Dish)
		{
			return;
		}

		if (TryReturnDish(Dish))
		{
			Grabber->Release();
		}

		return;
	}

	// 접시 액터를 플레이어가 쥐도록 설정
	AActor* Dish = Cast<AActor>(DishPool.Get());
	UPrimitiveComponent* Primitive = Dish->GetComponentByClass<UPrimitiveComponent>();
	Grabber->Grab(Primitive, Dish->GetActorLocation());
}