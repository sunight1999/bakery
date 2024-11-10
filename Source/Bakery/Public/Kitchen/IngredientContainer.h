// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "General/Tools/Pool.h"
#include "IngredientContainer.generated.h"

class UIngredientData;

/**
 * 
 */
UCLASS()
class BAKERY_API AIngredientContainer : public AGrabInteractableActor
{
	GENERATED_BODY()

public:
	AIngredientContainer();
	void Tick(float DeltaTime) override;

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override {}
	void OnInteract() override {}
	void OnExitInteract() override {}

	void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	void OnGrab() override {}
	void OnExitGrab() override {}

	void InitializeIngredient(UObject* IngredientObj);
	bool TryReturnIngredient(UObject* IngredientObj);

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Kitchen|Ingredient")
	UStaticMeshComponent* ContainerMesh;

	UPROPERTY(EditAnywhere, Category="Kitchen|Ingredient")
	UIngredientData* ContainedIngredient;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Ingredient")
	float IngredientMeshSizeMultiplier = 1.f;

	TPool IngredientPool;
};
