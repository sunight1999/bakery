// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "ExtraCountertop.generated.h"

class UIngredientData;
class UGrabberComponent;

/**
 * 
 */
UCLASS()
class BAKERY_API AExtraCountertop : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	AExtraCountertop();

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	virtual void OnExitInteract() override;

private:
	UPROPERTY(EditAnywhere, Category = "ExtraCountertop")
	TArray<UIngredientData*> ExtrasQuickMenuValues;

	UGrabberComponent* PendingGrabber = nullptr;
};
