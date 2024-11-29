// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/BakeryHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

#include "General/BakeryGameState.h"
#include "General/BakeryGameMode.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"

void UBakeryHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	BakeryGameState->OnMoneyChanged.AddUObject(this, &UBakeryHUDWidget::SetMoney);
	BakeryGameState->OnDayChanged.AddUObject(this, &UBakeryHUDWidget::SetDay);
	BakeryGameState->OnTimeChanged.AddUObject(this, &UBakeryHUDWidget::SetTime);
	BakeryGameState->OnTimeChanged.AddUObject(this, &UBakeryHUDWidget::SetTimeMarker);
	BakeryGameState->OnRatingChanged.AddUObject(this, &UBakeryHUDWidget::SetRating);

	// 평점 ProgressBar 레퍼런스 탐색
	for (int32 i = 0; i < 5; i++)
	{
		FString ProgressBarName = FString::Printf(TEXT("StarProgressBar_%d"), i);
		UProgressBar* ProgressBar = Cast<UProgressBar>(GetWidgetFromName(FName(ProgressBarName)));
		RatingProgressBars.Emplace(ProgressBar);
	}

	// 이 시점에선 GetCachedGeometry가 유효하지 않으므로 잠깐 기다렸다가 운영 시간 마커 위치 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBakeryHUDWidget::SetOperatingTimeMarker, 0.001f, false);
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

void UBakeryHUDWidget::SetRating(float Rating)
{
	int FullStars = (int)Rating;
	
	for (int i = 0; i < FullStars; i++)
	{
		RatingProgressBars[i]->SetPercent(1.f);
	}

	if (FullStars < 5)
	{
		RatingProgressBars[FullStars]->SetPercent(Rating - FullStars);
	}
}

/*
 * 중간 HUD 값 Setter
 */
UE::Slate::FDeprecateVector2DResult UBakeryHUDWidget::GetInternalDayClockSize() const
{
	return InternalDayClockCanvas->GetCachedGeometry().GetLocalSize();
}

void UBakeryHUDWidget::SetOperatingTimeMarker()
{
	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	SetOperatingTimeMarker(BakeryGameMode->GetBakeryOpenTime(), BakeryGameMode->GetBakeryCloseTime());
}

void UBakeryHUDWidget::SetOperatingTimeMarker(float OpenTime, float CloseTime)
{
	auto CachedDayClockSize = GetInternalDayClockSize();

	float DayClockWidth = CachedDayClockSize.X;
	float OpenTimeMarkerX = DayClockWidth * OpenTime / 1439;
	float CloseTimeMarkerX = DayClockWidth * CloseTime / 1439;
	
	SetMarkerPosition(BakeryOpenTimeMarkerImage, OpenTimeMarkerX);
	SetMarkerPosition(BakeryCloseTimeMarkerImage, CloseTimeMarkerX);
}

void UBakeryHUDWidget::SetTimeMarker(float CurrentTime)
{
	auto CachedDayClockSize = GetInternalDayClockSize();
	
	float DayClockWidth = CachedDayClockSize.X;
	float CurrentTimeMarkerX = DayClockWidth * CurrentTime / 1439;

	SetMarkerPosition(CurrentTimeMarkerImage, CurrentTimeMarkerX);
}

void UBakeryHUDWidget::SetMarkerPosition(UWidget* Widget, float X)
{
	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot))
	{
		FVector2D MarkerPosition = CanvasPanelSlot->GetPosition();
		MarkerPosition.X = X;
		CanvasPanelSlot->SetPosition(MarkerPosition);
	}
}

void UBakeryHUDWidget::AddAbnormalityMarker(const FAbnormalityEventData* AbnormalityEventData, int OccurrenceTime)
{
	auto CachedDayClockSize = GetInternalDayClockSize();
	float DayClockWidth = CachedDayClockSize.X;

	UImage* Marker = NewObject<UImage>(this);
	Marker->SetBrushFromTexture(AbnormalityEventData->Icon);

	InternalDayClockCanvas->AddChild(Marker);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Marker->Slot);
	CanvasSlot->SetAnchors(FAnchors(0.f, .5f));
	CanvasSlot->SetAlignment(FVector2D(.5f, .5f));
	CanvasSlot->SetPosition(FVector2D(.0f, .0f));
	CanvasSlot->SetSize(FVector2D(AbnormalityEventData->Icon->GetSizeX(), AbnormalityEventData->Icon->GetSizeY()));

	AbnormalityMarkerMap.Emplace(AbnormalityEventData, Marker);
	SetMarkerPosition(Marker, DayClockWidth * OccurrenceTime / 1439);
}

void UBakeryHUDWidget::RemoveAbnormalityMarker(const FAbnormalityEventData* AbnormalityEventData)
{
	UImage** Marker = AbnormalityMarkerMap.Find(AbnormalityEventData);
	if (Marker)
	{
		(*Marker)->SetVisibility(ESlateVisibility::Collapsed);
		(*Marker)->ConditionalBeginDestroy();
		AbnormalityMarkerMap.Remove(AbnormalityEventData);
	}
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
void UBakeryHUDWidget::SetTime(float Minute)
{
	int CurrentHour = Minute / 60;
	int CurrentMinute = Minute - CurrentHour * 60;
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
