// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Interactions/Interactables/InteractableComponent.h"

AGrabInteractableActor::AGrabInteractableActor()
{
	Interactable->ComponentTags.Add(INTERACTABLE_COMPONENT_GRABBALE);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetSimulatePhysics(true);
}

void AGrabInteractableActor::Tick(float DeltaTime)
{

}

void AGrabInteractableActor::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	// Interactor가 이 Interactable Actor를 잡도록 함
	FHitResult HitResult = InteractionInfo.HitResult;
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	Grabber->Grab(HitResult);
}

void AGrabInteractableActor::OnInteract()
{

}

void AGrabInteractableActor::OnExitInteract()
{

}
