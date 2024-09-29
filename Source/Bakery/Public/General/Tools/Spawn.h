// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawn.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawn : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BAKERY_API ISpawn
{
	GENERATED_BODY()

public:
	virtual void PostSpawn(AActor* Actor) = 0;
};
