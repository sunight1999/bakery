// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/TrashBin.h"

#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Kitchen/Ingredient.h"

ATrashBin::ATrashBin()
{
	TrashBinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrashBinMesh"));
	TrashBinMesh->SetupAttachment(RootComponent);
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
	if (GrabbedActor->IsA(AIngredient::StaticClass()))
	{
		Grabber->Release();
		GrabbedActor->Destroy(); // TODO: 추후 풀링 시스템 넣으면 Despawn으로 변경
	}
}
