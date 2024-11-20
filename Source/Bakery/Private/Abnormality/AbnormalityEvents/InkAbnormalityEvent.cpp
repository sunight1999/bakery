// Fill out your copyright notice in the Description page of Project Settings.


#include "Abnormality/AbnormalityEvents/InkAbnormalityEvent.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/Abnormality/InkAbnormalityWidget.h"

AInkAbnormalityEvent::AInkAbnormalityEvent()
{

}

void AInkAbnormalityEvent::BeginPlay()
{
	Super::BeginPlay();
}

void AInkAbnormalityEvent::Cause()
{
	if (!InkAbnormalityDrawing)
	{
		UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
		UUserWidget* UserWidget = UISubsystem->SetUIVisibility(FName("Ink"), ESlateVisibility::SelfHitTestInvisible);
		if (!UserWidget)
		{
			return;
		}

		InkAbnormalityDrawing = Cast<UInkAbnormalityWidget>(UserWidget);
	}

	InkAbnormalityDrawing->Initailize(InkImage, InkImageNum, InkMinDistance);
	GetWorld()->GetTimerManager().SetTimer(InkShakeTimer, InkAbnormalityDrawing, &UInkAbnormalityWidget::FadeOut, InkShakeInterval, true, InkShakeInterval);
}

void AInkAbnormalityEvent::Finish()
{
	GetWorld()->GetTimerManager().ClearTimer(InkShakeTimer);
}
