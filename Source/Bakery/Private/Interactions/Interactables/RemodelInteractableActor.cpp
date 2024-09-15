// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/RemodelInteractableActor.h"
#include "Interactions/Interactables/InteractableComponent.h"
#include "Interactions/InteractionDefines.h"

ARemodelInteractableActor::ARemodelInteractableActor()
{
	Interactable->ComponentTags.Add(INTERACTABLE_COMPONENT_REMODEL);
}
