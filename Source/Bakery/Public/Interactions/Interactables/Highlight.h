// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Highlight.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHighlight : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BAKERY_API IHighlight
{
	GENERATED_BODY()

public:
	virtual void OnEnterHighlight() = 0;
	virtual void OnExitHighlight() = 0;
};
