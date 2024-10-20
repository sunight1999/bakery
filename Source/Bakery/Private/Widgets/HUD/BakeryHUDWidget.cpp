// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/BakeryHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

#include "General/BakeryGameState.h"
#include "Widgets/Hall/WaitingTimeBarWidget.h"

void UBakeryHUDWidget::NativeConstruct()
{
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	BakeryGameState->OnMoneyChanged.AddUObject(this, &UBakeryHUDWidget::SetMoney);
	BakeryGameState->OnDayChanged.AddUObject(this, &UBakeryHUDWidget::SetDay);
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

	CustomerPredictCanvas->SetVisibility(HUDVisibility);
	GoalProgressBarCanvas->SetVisibility(HUDVisibility);
	DayProgressBarCanvas->SetVisibility(OppositeHUDVisibility);
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
void UBakeryHUDWidget::SetGoalProgress(float Progress)
{
	GoalProgressBar->SetPercentage(Progress);
}

void UBakeryHUDWidget::SetDayProgress(float Progress)
{
	DayProgressBar->SetPercentage(Progress);
}

/*
 * 오른쪽 HUD 값 Setter
 */
void UBakeryHUDWidget::SetDay(int Day)
{
	DayText->SetText(IntToFText(Day));
}
