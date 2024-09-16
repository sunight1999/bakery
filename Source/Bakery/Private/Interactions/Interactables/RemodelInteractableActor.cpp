// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/RemodelInteractableActor.h"
#include "Components/BoxComponent.h"

#include "Interactions/Interactables/InteractableComponent.h"
#include "Interactions/InteractionDefines.h"

ARemodelInteractableActor::ARemodelInteractableActor()
{
	Interactable->ComponentTags.Add(INTERACTABLE_COMPONENT_REMODELABLE);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
}

void ARemodelInteractableActor::Tick(float DeltaTime)
{
}


void ARemodelInteractableActor::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	// 영업 준비 시간인 경우 Grab 후 재설치 가능하게
}

void ARemodelInteractableActor::OnInteract()
{
}

void ARemodelInteractableActor::OnExitInteract()
{
}
