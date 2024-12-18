// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Components/BoxComponent.h"

#include "Interactions/InteractionDefines.h"

AGrabInteractableActor::AGrabInteractableActor()
{
	InteractionBox->SetCollisionProfileName(TEXT("Grabbable"));
}

void AGrabInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}
