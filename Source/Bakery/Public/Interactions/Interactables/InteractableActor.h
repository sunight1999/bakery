// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "Highlight.h"
#include "InteractableActor.generated.h"

class UInteractableComponent;
class UBoxComponent;

UCLASS()
class BAKERY_API AInteractableActor : public AActor, public IInteract, public IHighlight
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override { unimplemented(); }

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract() override { unimplemented(); }

	UFUNCTION(BlueprintCallable)
	virtual void OnExitInteract() override { unimplemented(); }

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterHighlight() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExitHighlight() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	UInteractableComponent* Interactable;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	UMaterialInterface* HighlightOverlayMaterial;
};
