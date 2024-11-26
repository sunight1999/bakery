// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BakeryHUDWidget.generated.h"

class UTextBlock;
class UCanvasPanel;
class UProgressBar;
class UImage;

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
	void SetRating(float Rating);

	/*
	 * 중간 HUD 값 Setter
	 */
	UE::Slate::FDeprecateVector2DResult GetInternalDayClockSize() const;
	void SetOperatingTimeMarker();
	void SetOperatingTimeMarker(float OpenTime, float CloseTime);
	void SetTimeMarker(float CurrentTime);
	void SetMarkerPosition(UWidget* Widget, float X);

	/*
	 * 오른쪽 HUD 값 Setter
	 */
	void SetDay(int Day);
	void SetTime(float Minute);

private:
	/*
	 * 왼쪽 HUD 변수
	 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyText;

	TArray<UProgressBar*> RatingProgressBars;

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
	UCanvasPanel* DayClockCanvas;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* InternalDayClockCanvas;

	UPROPERTY(meta = (BindWidget))
	UImage* DayClockImage;

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentTimeMarkerImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BakeryOpenTimeMarkerImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BakeryCloseTimeMarkerImage;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PrepareDisplay;

	FVector2D DayClockSize;

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
