// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BakeryHUDWidget.generated.h"

class UTextBlock;
class UCanvasPanel;
class UProgressBar;

/**
 *
 */
UCLASS()
class BAKERY_API UBakeryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	void SetHUDState(bool bIsOpened);
	bool GetHUDState() const { return bIsOpenedHUD; }
	
	FORCEINLINE FText IntToFText(int32 Value) { return FText::FromString(FString::FromInt(Value)); }
	FORCEINLINE FText FloatToFText(float Value) { return FText::FromString(FString::SanitizeFloat(Value)); }

	/*
	 * 왼쪽 HUD 값 Setter
	 */
	void SetMoney(int32 Money);
	void SetCustomerPredict(uint8 CustomerGroupNum, uint8 CustomerGroupConsistNum);

	/*
	 * 중간 HUD 값 Setter
	 */
	void SetSatisfactionProgress(float Progress);

	/*
	 * 오른쪽 HUD 값 Setter
	 */
	void SetDay(int Day);
	void SetTime(int Minute);

private:
	/*
	 * 왼쪽 HUD 변수
	 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyText;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CustomerPredictCanvas;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PredictGroupNumText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PredictGroupConsistNumText;

	/*
	 * 중간 HUD 변수
	 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SatisfactionBarCanvas;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* SatisfactionBar;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PrepareDisplay;

	/*
	 * 오른쪽 HUD 변수
	 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DayText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AMPMText;

	/*
	 * 기타
	 */
	bool bIsOpenedHUD = false;
};
