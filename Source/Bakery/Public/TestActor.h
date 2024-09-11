// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactions/Interactables/Interactable.h"
#include "TestActor.generated.h"

class UInteractableComponent;

UCLASS()
class BAKERY_API ATestActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ATestActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnEnterInteract() override;

	UFUNCTION(BlueprintCallable)
	void OnInteract() override;

	UFUNCTION(BlueprintCallable)
	void OnExitInteract() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UInteractableComponent* Interactable;
};
