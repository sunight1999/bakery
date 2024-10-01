// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Hall/WaitingTimeBarWidget.h"
#include "OrderWaitingTimeBarWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class BAKERY_API UDishWaitingTimeBarWidget : public UWaitingTimeBarWidget
{
	GENERATED_BODY()
	
public:
	void SetOrderImage(FString ImagePath);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* OrderImage;
};
