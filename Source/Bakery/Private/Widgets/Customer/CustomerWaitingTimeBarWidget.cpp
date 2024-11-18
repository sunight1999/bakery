// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Customer/CustomerWaitingTimeBarWidget.h"
#include "Components/Image.h"

void UCustomerWaitingTimeBarWidget::SetWaitingIconImage(FString ImagePath)
{
	UTexture2D* OrderTexture = LoadObject<UTexture2D>(nullptr, *ImagePath);
	if (!OrderTexture)
	{
		UE_LOG(LogTexture, Warning, TEXT("There is no '%s' texture asset."), *ImagePath);
	}

	WaitingIconImage->SetBrushFromTexture(OrderTexture);
}
