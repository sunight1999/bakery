// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Bakery/DessertExplainWidget.h"
#include "Kitchen/Data/RecipeData.h"
#include "Components/TextBlock.h"

void UDessertExplainWidget::SetDessert(const URecipeData* Recipe)
{
	DessertNameText->SetText(Recipe->GetExplainName());
	ExplainTitleText->SetText(Recipe->GetExplainTitle());
	ExplainText->SetText(Recipe->GetExplain());
}
