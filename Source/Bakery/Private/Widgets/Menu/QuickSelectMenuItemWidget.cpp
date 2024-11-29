// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Menu/QuickSelectMenuItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UQuickSelectMenuItemWidget::SetItemImage(UTexture2D* Image)
{
	ItemImage->SetBrushFromTexture(Image, true);
}

void UQuickSelectMenuItemWidget::SetItemNameBackgroundImage(UTexture2D* Image)
{
	ItemNameBackgroundImage->SetBrushFromTexture(Image, true);
}

void UQuickSelectMenuItemWidget::SetItemName(FName Name)
{
	ItemNameText->SetText(FText::FromName(Name));
}

void UQuickSelectMenuItemWidget::SetFocus(bool bIsFocused)
{
	if (bIsFocused)
	{
		PlayAnimation(FocusingAnim);
	}
	else
	{
		PlayAnimation(LoseFocusingAnim);
	}
}
