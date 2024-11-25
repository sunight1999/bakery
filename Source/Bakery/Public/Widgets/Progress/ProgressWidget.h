// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class BAKERY_API UProgressWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	float GetPercentage() const { return Percentage; }
	void SetPercentage(float InPercentage) { Percentage = InPercentage; }

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	float Percentage = 0.f;
};
