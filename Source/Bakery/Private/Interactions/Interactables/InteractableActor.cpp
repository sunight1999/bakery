// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/InteractableActor.h"
#include "Components/BoxComponent.h"

#include "Interactions/Interactables/InteractableComponent.h"
#include "Interactions/InteractionDefines.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Interactable = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	Interactable->OnEnterInteractDelegate.AddDynamic(this, &AInteractableActor::OnEnterInteract);
	Interactable->OnInteractDelegate.AddDynamic(this, &AInteractableActor::OnInteract);
	Interactable->OnExitInteractDelegate.AddDynamic(this, &AInteractableActor::OnExitInteract);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetCollisionProfileName(TEXT("Interactable"));
	RootComponent = InteractionBox;

	HighlightOverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/ETC/Overlays/MI_InteractableOverlay"));
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableActor::OnEnterHighlight()
{
	if (!HighlightOverlayMaterial)
	{
		return;
	}

	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);

	for (auto MeshComponent : MeshComponents)
	{
		MeshComponent->SetOverlayMaterial(HighlightOverlayMaterial);
	}
}

void AInteractableActor::OnExitHighlight()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);

	for (auto MeshComponent : MeshComponents)
	{
		MeshComponent->SetOverlayMaterial(nullptr);
	}
}
