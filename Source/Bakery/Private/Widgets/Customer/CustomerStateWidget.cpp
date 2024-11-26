// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Customer/CustomerStateWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UCustomerStateWidget::SetProgressBarColor(UProgressBar* ProgressBar, const FSlateColor& NewColor)
{
	FProgressBarStyle ProgressBarStyle = ProgressBar->GetWidgetStyle();
	ProgressBarStyle.FillImage.TintColor = NewColor;

	ProgressBar->SetWidgetStyle(ProgressBarStyle);
}

/*
 * 대기 시간 관련
 */
void UCustomerStateWidget::ResetWaitingTimeBar()
{
	SetProgressBarColor(WaitingTimeBar, WaitingNiceStateColor);
	WaitingState = EWaitingState::Nice;
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
	switch (WaitingState)
	{
	case EWaitingState::Nice:
		if (WaitingTimeBarPercentage <= WaitingNotBadStateThreshold)
		{
			WaitingState = EWaitingState::NotBad;
			SetProgressBarColor(WaitingTimeBar, WaitingNotBadStateColor);
			QueuePlayAnimation(WaitingTimeBarShakeAnim);
			OnWaitingStateChanged.Broadcast(WaitingState);
		}
		break;

	case EWaitingState::NotBad:
		if (WaitingTimeBarPercentage <= WaitingBadStateThreshold)
		{
			WaitingState = EWaitingState::Bad;
			SetProgressBarColor(WaitingTimeBar, WaitingBadStateColor);
			QueuePlayAnimation(WaitingTimeBarShakeAnim, 0.f, 2);
			OnWaitingStateChanged.Broadcast(WaitingState);
		}
		break;

	case EWaitingState::Bad:
		break;
	}
}

/*
 * 공포 수치 관련
 */
void UCustomerStateWidget::ResetFearBar()
{
	SetProgressBarColor(FearBar, FearNiceStateColor);
	FearState = EWaitingState::Nice;
	FearBarPercentage = 1.f;
}

void UCustomerStateWidget::SetFearBarVisibility(bool bVisible)
{
	if (bVisible)
	{
		QueuePlayAnimation(FearBarAppearAnim);
	}
	else
	{
		QueuePlayAnimationReverse(FearBarAppearAnim);
		ResetFearBar();
	}

	bIsFearBarVisible = bVisible;
}

void UCustomerStateWidget::SetFearBarPercentage(float InPercentage)
{
	FearBarPercentage = InPercentage;

	// Percentage에 따라 색 변경
	switch (FearState)
	{
	case EWaitingState::Nice:
		if (WaitingTimeBarPercentage >= FearNotBadStateThreshold)
		{
			FearState = EWaitingState::NotBad;
			SetProgressBarColor(FearBar, FearNotBadStateColor);
			QueuePlayAnimation(FearBarShakeAnim);
			OnFearStateChanged.Broadcast(FearState);
		}
		break;

	case EWaitingState::NotBad:
		if (FearBarPercentage >= FearBadStateThreshold)
		{
			FearState = EWaitingState::Bad;
			SetProgressBarColor(FearBar, FearBadStateColor);
			QueuePlayAnimation(FearBarShakeAnim, 0.f, 2);
			OnFearStateChanged.Broadcast(FearState);
		}
		break;

	case EWaitingState::Bad:
		break;
	}
}
