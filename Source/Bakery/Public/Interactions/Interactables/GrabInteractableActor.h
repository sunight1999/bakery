// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "GrabInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API AGrabInteractableActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	AGrabInteractableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

protected:

private:

};
