// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactions/Interactables/GrabInteractableActor.h"
#include "Dish.generated.h"

class AIngredient;
class UNiagaraComponent;

UCLASS()
class BAKERY_API ADish : public AGrabInteractableActor
{
	GENERATED_BODY()
	
public:	
	ADish();

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override { }
	virtual void OnInteract() override { }
	virtual void OnExitInteract() override { }

	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	virtual void OnGrab() override { }
	virtual void OnExitGrab() override { }

	virtual void OnEnterHighlight() override;
	virtual void OnExitHighlight() override;

	bool IsClean() const { return bIsClean; }
	bool IsLostDish() const { return bIsLostDish; }
	bool IsSettable(AIngredient* Ingredient);
	bool SetDessert(AIngredient* Ingredient);
	AIngredient* PeekDessert();

	void Eat();
	void Wash();
	void FlyAway(float InMoveSpeed, float InFlySpeed, float InAmplitude, float InMoveInterval);
	void Caught();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Kitchen|Dish")
	UStaticMeshComponent* DishMesh;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Dish")
	float DishThickness = 10.f;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Dish")
	int HandWashingTime = 5;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Dish")
	FVector OriginInteractionBoxExtent;

	UPROPERTY(VisibleAnywhere, Category = "Kitchen|Dish")
	UNiagaraComponent* FlyAwayEffect;

	AIngredient* Dessert = nullptr;

	bool bIsClean = true;
	int CurrentWashingTime = 0;

	bool bIsFlying = false;
	bool bIsLostDish = false;
};
