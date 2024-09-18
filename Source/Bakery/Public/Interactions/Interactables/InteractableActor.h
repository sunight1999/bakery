// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "InteractableActor.generated.h"

class UInteractableComponent;
class UBoxComponent;

UCLASS()
class BAKERY_API AInteractableActor : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExitInteract() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	UInteractableComponent* Interactable;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	UBoxComponent* InteractionBox;
};
