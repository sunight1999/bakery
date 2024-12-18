// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "Grab.h"
#include "GrabInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API AGrabInteractableActor : public AInteractableActor, public IGrab
{
	GENERATED_BODY()
	
public:
	AGrabInteractableActor();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override { unimplemented(); }

	UFUNCTION(BlueprintCallable)
	virtual void OnGrab(float DeltaTime) override { unimplemented(); }

	UFUNCTION(BlueprintCallable)
	virtual void OnExitGrab() override { unimplemented(); }

protected:
	virtual void BeginPlay() override;
};
