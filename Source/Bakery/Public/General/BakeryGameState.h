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
	// 자금 관련
	int32 GetMoney() const { return Money; }
	void AddMoney(int32 InMoney);
	
	// 일자 관련
	int32 GetDay() const { return Day; }
	void AddDay(int32 InDay);
	void AddDay();

	// 시간 관련
	void SetTime(float Minute);
	void AddTime();

	// 평점 관련
	void AddRating(float InRating);

	// 요리 완성 경험 관련
	void SetCookedRecipe(FName RecipeName) { CookedRecipes.Emplace(RecipeName); }
	bool IsCookedRecipe(FName RecipeName) { return CookedRecipes.Contains(RecipeName); }

	float GetElapsedTime() const { return ElapsedTime; }

	FMoneyChangedDelegate OnMoneyChanged;
	FDayChangedDelegate OnDayChanged;
	FTimeChangedDelegate OnTimeChanged;
	FRatingChangedDelegate OnRatingChanged;

protected:
	void BeginPlay() override;

private:
	// 보유 자금
	int32 Money;
	// 영업일
	int32 Day;
	// 경과 시간 (단위: 분)
	float ElapsedTime;
	// 고객 평점
	float Rating = 0.f;
	// 요리 완성 경험 여부
	TSet<FName> CookedRecipes;

	EBakeryState BakeryState = EBakeryState::Closed;
};
