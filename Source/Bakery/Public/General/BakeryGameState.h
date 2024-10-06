// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BakeryDefines.h"
#include "BakeryGameState.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ABakeryGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	EBakeryState GetBakeryState() const { return BakeryState; }
	void SetBakeryState(EBakeryState NewState) { BakeryState = NewState; }

private:
	EBakeryState BakeryState = EBakeryState::Preparing;
};
