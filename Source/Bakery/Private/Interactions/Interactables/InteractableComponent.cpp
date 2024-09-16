// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/InteractableComponent.h"
#include "Interactions/InteractionDefines.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
