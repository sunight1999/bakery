// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Customer/OrderWaitingTimeBarWidget.h"
#include "Components/Image.h"

void UDishWaitingTimeBarWidget::SetOrderImage(FString ImagePath)
{
	UTexture2D* OrderTexture = LoadObject<UTexture2D>(nullptr, *ImagePath);
	if (!OrderTexture)
	{
		UE_LOG(LogTexture, Warning, TEXT("There is no '%s' texture asset."), *ImagePath);
	}

	OrderImage->SetBrushFromTexture(OrderTexture);
}
