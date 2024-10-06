// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BakeryGameMode.h"
#include "NormalBakeryGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ANormalBakeryGameMode : public ABakeryGameMode
{
	GENERATED_BODY()
	
public:
	void Disappointed(APawn* Pawn) override;

protected:
	void BeginPlay() override;
};
