// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "General/Tools/Pool.h"
#include "DishContainer.generated.h"

class ADish;

UCLASS()
class BAKERY_API ADishContainer : public AGrabInteractableActor
{
	GENERATED_BODY()
	
public:	
	ADishContainer();

	void End();

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override {}
	virtual void OnInteract(float DeltaTime) override {}
	virtual void OnExitInteract() override {}

	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	virtual void OnGrab(float DeltaTime) override {}
	virtual void OnExitGrab() override {}

	void InitializeDish(UObject* DishObj);
	bool TryReturnDish(UObject* DishObj);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "DishContainer")
	TSubclassOf<ADish> DishClass;

	TPool DishPool;
};
