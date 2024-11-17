// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Hall/WaitingTimeBarWidget.h"
#include "CustomerWaitingTimeBarWidget.generated.h"

#define WAITING_ORDER_ICON_BASE_PATH FString("/Game/Textures/Icon/Order/")
#define WAITING_ORDER_ICON_ORDERING_PATH WAITING_ORDER_ICON_BASE_PATH + FString("Bell")

class UImage;

/**
 * 
 */
UCLASS()
class BAKERY_API UCustomerWaitingTimeBarWidget : public UWaitingTimeBarWidget
{
	GENERATED_BODY()
	
public:
	void SetWaitingIconImage(FString ImagePath);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* WaitingIconImage;
};
