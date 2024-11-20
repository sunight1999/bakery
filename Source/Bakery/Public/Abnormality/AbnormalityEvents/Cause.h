// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Cause.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCause : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BAKERY_API ICause
{
	GENERATED_BODY()

public:
	virtual void Cause() = 0;
	virtual void Finish() = 0;
};
