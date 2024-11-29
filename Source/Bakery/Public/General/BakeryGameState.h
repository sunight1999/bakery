// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BakeryDefines.h"
#include "BakeryGameState.generated.h"

class UBakerySaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FMoneyChangedDelegate, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FDayChangedDelegate, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FTimeChangedDelegate, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FRatingChangedDelegate, float)

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

	/*
	 * 게임 데이터 Getter & Setter
	 */
	int32 GetMoney() const { return Money; }
	void AddMoney(int32 InMoney);
	int32 GetDay() const { return Day; }
	void AddDay(int32 InDay);
	void AddDay();

	void SetTime(float Minute);
	void AddTime();

	void AddRating(float InRating);

	float GetElapsedTime() const { return ElapsedTime; }

	FMoneyChangedDelegate OnMoneyChanged;
	FDayChangedDelegate OnDayChanged;
	FTimeChangedDelegate OnTimeChanged;
	FRatingChangedDelegate OnRatingChanged;

protected:
	void BeginPlay() override;

private:
	// 보유 자본
	int32 Money;
	// 영업일
	int32 Day;
	// 경과 시간 (단위: 분)
	float ElapsedTime;
	// 고객 평점
	float Rating = 0.f;

	EBakeryState BakeryState = EBakeryState::Closed;
};
