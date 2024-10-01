// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Hall/WaitingTimeBarWidget.h"
#include "Components/ProgressBar.h"

void UWaitingTimeBarWidget::Reset()
{
	WaitingTimeBar->SetPercent(1.f);
}

void UWaitingTimeBarWidget::SetPercentage(float CurrentValue, float MaxValue)
{
	if (MaxValue == 0)
	{
		WaitingTimeBar->SetPercent(0.f);
	}
	else
	{
		WaitingTimeBar->SetPercent(CurrentValue / MaxValue);
	}
}

void UWaitingTimeBarWidget::SetPercentage(float Percentage)
{
	WaitingTimeBar->SetPercent(Percentage);
}
