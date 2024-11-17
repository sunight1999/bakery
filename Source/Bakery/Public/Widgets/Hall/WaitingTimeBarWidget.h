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
	UFUNCTION(BlueprintCallable)
	float GetWaitingTimeBarPercentage() const { return Percentage; }

	void Reset() { Percentage = 1.f; }
	void SetPercentage(float InPercentage) { Percentage = InPercentage; }

private:
	float Percentage = 1.f;
};
