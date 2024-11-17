// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/BakeryHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

#include "General/BakeryGameState.h"
#include "Widgets/Hall/WaitingTimeBarWidget.h"

void UBakeryHUDWidget::NativeConstruct()
{
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	BakeryGameState->OnMoneyChanged.AddUObject(this, &UBakeryHUDWidget::SetMoney);
	BakeryGameState->OnDayChanged.AddUObject(this, &UBakeryHUDWidget::SetDay);
	BakeryGameState->OnTimeChanged.AddUObject(this, &UBakeryHUDWidget::SetTime);
}

void UBakeryHUDWidget::SetHUDState(bool bIsOpened)
{
	if (bIsOpenedHUD == bIsOpened)
	{
		return;
	}

	bIsOpenedHUD = bIsOpened;
	ESlateVisibility HUDVisibility;
	ESlateVisibility OppositeHUDVisibility;

	if (bIsOpenedHUD)
	{
		HUDVisibility = ESlateVisibility::Collapsed;
		OppositeHUDVisibility = ESlateVisibility::SelfHitTestInvisible;
	}
	else
	{
		HUDVisibility = ESlateVisibility::SelfHitTestInvisible;
		OppositeHUDVisibility = ESlateVisibility::Collapsed;
	}

	// CustomerPredictCanvas의 경우 아직 미구현 기능이므로 Collapsed 처리
	CustomerPredictCanvas->SetVisibility(ESlateVisibility::Collapsed);
	SatisfactionBarCanvas->SetVisibility(OppositeHUDVisibility);
	PrepareDisplay->SetVisibility(HUDVisibility);
}

/*
 * 왼쪽 HUD 값 Setter
 */
void UBakeryHUDWidget::SetMoney(int32 Money)
{
	MoneyText->SetText(IntToFText(Money));
}

void UBakeryHUDWidget::SetCustomerPredict(uint8 CustomerGroupNum, uint8 CustomerGroupConsistNum)
{
	PredictGroupNumText->SetText(IntToFText(CustomerGroupNum));
	PredictGroupConsistNumText->SetText(IntToFText(CustomerGroupConsistNum));
}

/*
 * 중간 HUD 값 Setter
 */
void UBakeryHUDWidget::SetSatisfactionProgress(float Progress)
{
	SatisfactionBar->SetPercent(Progress);
}

/*
 * 오른쪽 HUD 값 Setter
 */
void UBakeryHUDWidget::SetDay(int Day)
{
	DayText->SetText(FText::FromString(FString::Printf(TEXT("영업 %d일차"), Day)));
}

/// <summary>
/// 경과한 분에 맞게 시간 표시
/// </summary>
/// <param name="Minute">현재 하루 중 경과한 분. 0 ~ 1439 (00:00 ~ 23:59)</param>
void UBakeryHUDWidget::SetTime(int Minute)
{
	int CurrentHour = Minute / 60;
	int CurrentMinute = Minute % 60;
	if (CurrentHour > 12)
	{
		CurrentHour -= 12;
	}

	TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), CurrentHour, CurrentMinute)));

	if (Minute < 720)
	{
		AMPMText->SetText(FText::FromString("AM"));
	}
	else
	{
		AMPMText->SetText(FText::FromString("PM"));
	}
}
