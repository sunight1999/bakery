// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "RemodelInteractableActor.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class BAKERY_API ARemodelInteractableActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	ARemodelInteractableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Interaction|Remodel")
	UBoxComponent* InteractionBox;
};
