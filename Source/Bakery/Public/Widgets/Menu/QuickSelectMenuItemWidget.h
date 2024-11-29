// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSelectMenuItemWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BAKERY_API UQuickSelectMenuItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemImage(UTexture2D* Image);
	void SetItemNameBackgroundImage(UTexture2D* Image);
	void SetItemName(FName Name);

	void SetFocus(bool bIsFocused);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemNameBackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FocusingAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* LoseFocusingAnim;
};
