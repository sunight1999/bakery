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
	RootComponent = InteractionBox;
	InteractionBox->SetCollisionProfileName(TEXT("Interactable"));
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}


void AInteractableActor::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	unimplemented();
}

void AInteractableActor::OnInteract()
{
	unimplemented();
}

void AInteractableActor::OnExitInteract()
{
	unimplemented();
}
