// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BakerySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API UBakerySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 Money;

	UPROPERTY()
	int32 Day;

	UPROPERTY()
	float Rating;
};
