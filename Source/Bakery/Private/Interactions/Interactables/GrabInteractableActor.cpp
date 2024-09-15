// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Interactions/Interactables/InteractableComponent.h"
#include "Interactions/InteractionDefines.h"

AGrabInteractableActor::AGrabInteractableActor()
{
	Interactable->ComponentTags.Add(INTERACTABLE_COMPONENT_GRAB);
}

void AGrabInteractableActor::Tick(float DeltaTime)
{

}

void AGrabInteractableActor::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	
}

void AGrabInteractableActor::OnInteract()
{

}

void AGrabInteractableActor::OnExitInteract()
{

}
