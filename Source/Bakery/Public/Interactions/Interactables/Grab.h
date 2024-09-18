// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Grab.generated.h"

struct FInteractionInfo;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGrab : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BAKERY_API IGrab
{
	GENERATED_BODY()

public:
	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) = 0;
	virtual void OnGrab() = 0;
	virtual void OnExitGrab() = 0;
};
