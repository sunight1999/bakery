// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "General/Tools/Spawner.h"
#include "CustomerSpawner.generated.h"

class AHallManager;
class URecipeSubsystem;

/**
 * 
 */
UCLASS()
class BAKERY_API ACustomerSpawner : public ASpawner
{
	GENERATED_BODY()
	
public:
	ACustomerSpawner();

	void PostSpawn(AActor* Actor) override;

protected:
	virtual void BeginPlay() override;

private:
	AHallManager* HallManager;
	URecipeSubsystem* RecipeSubsystem;
};
