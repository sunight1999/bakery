// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingTimeBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class BAKERY_API UWaitingTimeBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Reset();
	void SetPercentage(float CurrentValue, float MaxValue);
	void SetPercentage(float Percentage);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* WaitingTimeBar;
};
