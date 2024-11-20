// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "InkAbnormalityEvent.generated.h"

class UInkAbnormalityWidget;
class UUISubsystem;

/**
 * 
 */
UCLASS()
class BAKERY_API AInkAbnormalityEvent : public AAbnormalityEvent
{
	GENERATED_BODY()
	
public:
	AInkAbnormalityEvent();

	virtual void Cause() override;
	virtual void Finish() override;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="AbnormalityEvent|Ink")
	UTexture2D* InkImage;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Ink")
	int InkImageNum = 4;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Ink")
	float InkShakeInterval = 20.f;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Ink")
	int InkMinDistance = 300.f;

	UInkAbnormalityWidget* InkAbnormalityDrawing;
	FTimerHandle InkShakeTimer;
};
