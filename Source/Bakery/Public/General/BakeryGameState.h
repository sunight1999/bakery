// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BakeryDefines.h"
#include "BakeryGameState.generated.h"

class UBakerySaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FMoneyChangedDelegate, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FDayChangedDelegate, int32)

/**
 * 
 */
UCLASS()
class BAKERY_API ABakeryGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	EBakeryState GetBakeryState() const { return BakeryState; }
	void SetBakeryStateOpened() { BakeryState = EBakeryState::Opened; }
	void SetBakeryStateClosed() { BakeryState = EBakeryState::Closed; }

	void LoadGame();
	void SaveGame();

	void AddMoney(int32 InMoney);
	void AddDay(int32 InDay);
	void AddDay();

	FMoneyChangedDelegate OnMoneyChanged;
	FDayChangedDelegate OnDayChanged;

protected:
	void BeginPlay() override;

private:
	int32 Money;
	int32 Day;

	EBakeryState BakeryState = EBakeryState::Closed;
};
