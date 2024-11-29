// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettleSalesWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

/**
 * 
 */
UCLASS()
class BAKERY_API USettleSalesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetDay(int Day);
	void SetEarnedMoney(int Money);
	void SetRatings(TArray<int> Ratings);
	void SetRatingStars(float Rating);

protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DayText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EarnedMoneyText;

	TArray<UTextBlock*> RatingTexts;
	TArray<UProgressBar*> RatingBars;
};
