// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Abnormality/InkAbnormalityWidget.h"
#include "Engine/GameViewportClient.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UInkAbnormalityWidget::NativeConstruct()
{
	FWidgetAnimationDynamicEvent FadeOutFinished;
	FadeOutFinished.BindDynamic(this, &UInkAbnormalityWidget::OnFadeOutFinished);
	
	BindToAnimationFinished(InkFadeOutAnim, FadeOutFinished);
}

void UInkAbnormalityWidget::Initailize(UTexture2D* InkImage, int InkNum, float InInkImageMinDistance)
{
	if (InkImages.Num() == InkNum)
	{
		return;
	}

	InkImages.Empty();

	for (int i = 0; i < InkNum; i++)
	{
		UImage* Ink = NewObject<UImage>(this);
		Ink->SetBrushFromTexture(InkImage);

		InkImageContainer->AddChild(Ink);
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Ink->Slot);
		CanvasSlot->SetAnchors(FAnchors(.5f, .5f));
		CanvasSlot->SetAlignment(FVector2D(.5f, .5f));
		CanvasSlot->SetPosition(FVector2D(.0f, .0f));
		CanvasSlot->SetSize(FVector2D(InkImage->GetSizeX() / 2, InkImage->GetSizeY() / 2));

		InkImages.Emplace(Ink);
	}

	InkImageMinDistance = InInkImageMinDistance * InInkImageMinDistance;

	Shake();
	FadeIn();
}

void UInkAbnormalityWidget::Shake()
{
	FIntPoint ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	for (int i = 0; i < InkImages.Num(); i++)
	{
		UImage* InkImage = InkImages[i];
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InkImage->Slot);
		InkImage->SetRenderTransformAngle(FMath::RandRange(-180.f, 180.f));
		InkImage->SetRenderScale(FVector2D(FMath::RandRange(.8f, 1.2f)));

		float X = FMath::RandRange(-ViewportSize.X, ViewportSize.X);
		float Y = FMath::RandRange(-ViewportSize.Y, ViewportSize.Y);
		FVector2D InkImagePosition = FVector2D(X, Y);

		for (int j = 0; j < i; j++)
		{
			UCanvasPanelSlot* PrevInkCanvasSlot = Cast<UCanvasPanelSlot>(InkImages[j]->Slot);
			if (FVector2D::DistSquared(InkImagePosition, PrevInkCanvasSlot->GetPosition()) <= InkImageMinDistance)
			{
				X = FMath::RandRange(-ViewportSize.X, ViewportSize.X);
				Y = FMath::RandRange(-ViewportSize.Y, ViewportSize.Y);
				InkImagePosition = FVector2D(X, Y);

				j = -1;
			}
		}

		CanvasSlot->SetPosition(InkImagePosition);
	}
}

void UInkAbnormalityWidget::FadeIn()
{
	bIsVisible = true;
	QueuePlayAnimation(InkFadeInAnim);
}

void UInkAbnormalityWidget::FadeOut()
{
	bIsVisible = false;
	QueuePlayAnimation(InkFadeOutAnim);
}

void UInkAbnormalityWidget::OnFadeOutFinished()
{
	Shake();
	QueuePlayAnimation(InkFadeInAnim);
}
