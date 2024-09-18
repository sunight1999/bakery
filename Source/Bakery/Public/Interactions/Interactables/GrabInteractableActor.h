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
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnGrab() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExitGrab() override;

private:
	UPROPERTY(EditAnywhere, Category="Interaction|Remodel")
	bool bIsRemodelable = false;
};
