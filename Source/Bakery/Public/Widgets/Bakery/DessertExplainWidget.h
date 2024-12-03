// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DessertExplainWidget.generated.h"

class URecipeData;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BAKERY_API UDessertExplainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDessert(const URecipeData* Recipe);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DessertNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExplainTitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExplainText;
};
