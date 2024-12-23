// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

struct FInteractionInfo;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BAKERY_API IInteract
{
	GENERATED_BODY()

public:
	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) = 0;
	virtual void OnInteract(float DeltaTime) = 0;
	virtual void OnExitInteract() = 0;
};
