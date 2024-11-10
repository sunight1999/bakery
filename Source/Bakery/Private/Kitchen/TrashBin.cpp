// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/TrashBin.h"
#include "EngineUtils.h"

#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/IngredientContainer.h"
#include "Kitchen/Dish.h"

ATrashBin::ATrashBin()
{
	TrashBinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrashBinMesh"));
	TrashBinMesh->SetupAttachment(RootComponent);
}

void ATrashBin::BeginPlay()
{
	for (TActorIterator<AIngredientContainer> It(GetWorld()); It; ++It)
	{
		IngredientContainers.Emplace(*It);
	}
}

void ATrashBin::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();
	if (!Grabbed)
	{
		return;
	}

	AActor* GrabbedActor = Grabbed->GetOwner();
	if (GrabbedActor->IsA(ADish::StaticClass()))
	{
		ADish* Dish = Cast<ADish>(GrabbedActor);
		if (Dish->PeekDessert())
		{
			Dish->Eat();
		}

		return;
	}

	if (!GrabbedActor->IsA(AIngredient::StaticClass()))
	{
		return;
	}

	bool bIsReturned = false;
	for (AIngredientContainer* Container : IngredientContainers)
	{
		bIsReturned = Container->TryReturnIngredient(GrabbedActor);
		if (bIsReturned)
		{
			break;
		}
	}

	if (!bIsReturned)
	{
		return;
	}

	Grabber->Release();
}
