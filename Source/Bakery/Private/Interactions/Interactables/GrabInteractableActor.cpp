// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Components/BoxComponent.h"

#include "Interactions/InteractionDefines.h"
#include "Interactions/Interactables/InteractableComponent.h"

AGrabInteractableActor::AGrabInteractableActor()
{
	Interactable->OnEnterGrabDelegate.AddDynamic(this, &AGrabInteractableActor::OnEnterGrab);
	Interactable->OnGrabDelegate.AddDynamic(this, &AGrabInteractableActor::OnGrab);
	Interactable->OnExitGrabDelegate.AddDynamic(this, &AGrabInteractableActor::OnExitGrab);

	InteractionBox->SetCollisionProfileName(TEXT("Grabbable"));
}

void AGrabInteractableActor::Tick(float DeltaTime)
{

}

void AGrabInteractableActor::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	unimplemented();
}

void AGrabInteractableActor::OnGrab()
{
	unimplemented();
}

void AGrabInteractableActor::OnExitGrab()
{
	unimplemented();
}
