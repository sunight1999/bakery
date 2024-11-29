// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Bakery/SettleSalesWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void USettleSalesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < 5; i++)
	{
		// 평점 UTextBlock 레퍼런스 탐색
		FString RatingTextName = FString::Printf(TEXT("RatingText_%d"), i);
		UTextBlock* RatingText = Cast<UTextBlock>(GetWidgetFromName(FName(RatingTextName)));
		RatingTexts.Emplace(RatingText);

		// 평점 ProgressBar 레퍼런스 탐색
		FString ProgressBarName = FString::Printf(TEXT("StarProgressBar_%d"), i);
		UProgressBar* ProgressBar = Cast<UProgressBar>(GetWidgetFromName(FName(ProgressBarName)));
		RatingBars.Emplace(ProgressBar);
	}
}

void USettleSalesWidget::SetDay(int Day)
{
	DayText->SetText(FText::FromString(FString::Printf(TEXT("영업 %d일차"), Day)));
}

void USettleSalesWidget::SetEarnedMoney(int Money)
{
	EarnedMoneyText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Money)));
}

void USettleSalesWidget::SetRatings(TArray<int> Ratings)
{
	int TotalCount = 0;
	int TotalScore = 0;
	for (int i = 0; i < Ratings.Num(); i++)
	{
		int Count = Ratings[i];
		RatingTexts[i]->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
		TotalCount += Count;
		TotalScore += Count * (i + 1);
	}

	SetRatingStars((float)TotalScore / TotalCount);
}

void USettleSalesWidget::SetRatingStars(float Rating)
{
	int FullStars = (int)Rating;

	for (int i = 0; i < FullStars; i++)
	{
		RatingBars[i]->SetPercent(1.f);
	}

	if (FullStars < 5)
	{
		RatingBars[FullStars]->SetPercent(Rating - FullStars);
	}
}