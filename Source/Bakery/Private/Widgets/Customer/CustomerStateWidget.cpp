// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Customer/CustomerStateWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

/*
 * 대기 시간 관련
 */
void UCustomerStateWidget::ResetWaitingTimeBar()
{
	WaitingState = EWaitingState::None;
	WaitingTimeBarPercentage = 1.f;
}

void UCustomerStateWidget::SetWaitingIconImage(FString ImagePath)
{
	UTexture2D* OrderTexture = LoadObject<UTexture2D>(nullptr, *ImagePath);
	if (!OrderTexture)
	{
		UE_LOG(LogTexture, Warning, TEXT("There is no '%s' texture asset."), *ImagePath);
	}

	WaitingIconImage->SetBrushFromTexture(OrderTexture);
}

void UCustomerStateWidget::SetWaitingTimeBarColor(const FSlateColor& NewColor)
{
	FProgressBarStyle ProgressBarStyle = WaitingTimeBar->GetWidgetStyle();
	ProgressBarStyle.FillImage.TintColor = NewColor;

	WaitingTimeBar->SetWidgetStyle(ProgressBarStyle);
}

void UCustomerStateWidget::SetWaitingTimeBarVisibility(bool bVisible)
{
	if (bVisible)
	{
		QueuePlayAnimation(WaitingTimeBarAppearAnim);
	}
	else
	{
		QueuePlayAnimationReverse(WaitingTimeBarAppearAnim);
	}

	ResetWaitingTimeBar();
	bIsWaitingTimeBarVisible = bVisible;
}

void UCustomerStateWidget::SetWaitingTimeBarPercentage(float InPercentage)
{
	WaitingTimeBarPercentage = InPercentage;

	// Percentage에 따라 색 변경
	EWaitingState NewWaitingState = EWaitingState::None;
	FColor& NewColor = WaitingNiceStateColor;

	if (WaitingTimeBarPercentage <= WaitingBadStateThreshold)
	{
		NewWaitingState = EWaitingState::Bad;
		NewColor = WaitingBadStateColor;
	}
	else if (WaitingTimeBarPercentage <= WaitingNotBadStateThreshold)
	{
		NewWaitingState = EWaitingState::NotBad;
		NewColor = WaitingNotBadStateColor;
	}
	else
	{
		NewWaitingState = EWaitingState::Nice;
		NewColor = WaitingNiceStateColor;
	}

	if (WaitingState != NewWaitingState)
	{
		WaitingState = NewWaitingState;
		SetWaitingTimeBarColor(NewColor);

		if (WaitingState == EWaitingState::Bad)
		{
			QueuePlayAnimation(WaitingTimeBarShakeAnim, 0.f, 2);
		}
		else if (WaitingState == EWaitingState::NotBad)
		{
			QueuePlayAnimation(WaitingTimeBarShakeAnim);
		}
	}
}

/*
 * 공포 수치 관련
 */
void UCustomerStateWidget::SetFearBarVisibility(bool bVisible)
{
	if (bVisible)
	{
		QueuePlayAnimation(FearBarAppearAnim);
	}
	else
	{
		QueuePlayAnimationReverse(FearBarAppearAnim);
	}

	bIsFearBarVisible = bVisible;
}

void UCustomerStateWidget::SetFearBarPercentage(float InPercentage)
{
	FearBarPercentage = InPercentage;
}
