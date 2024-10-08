// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "TrashBin.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ATrashBin : public AGrabInteractableActor
{
	GENERATED_BODY()
	
public:
	ATrashBin();

	void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	void OnGrab() override {}
	void OnExitGrab() override {}

private:
	UPROPERTY(VisibleAnywhere, Category = "TrashBin")
	UStaticMeshComponent* TrashBinMesh;
};
