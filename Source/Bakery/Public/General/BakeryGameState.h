// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BakeryDefines.h"
#include "BakeryGameState.generated.h"

class UBakerySaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FMoneyChangedDelegate, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FDayChangedDelegate, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FTimeChangedDelegate, int32)

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
	void SetTime(int Minute);
	void AddTime();

	int32 GetElapsedTime() const { return ElapsedTime; }

	FMoneyChangedDelegate OnMoneyChanged;
	FDayChangedDelegate OnDayChanged;
	FTimeChangedDelegate OnTimeChanged;

protected:
	void BeginPlay() override;

private:
	// 보유 자본
	int32 Money;
	// 영업일
	int32 Day;
	// 경과 시간 (단위: 분)
	int32 ElapsedTime;

	EBakeryState BakeryState = EBakeryState::Closed;
};
