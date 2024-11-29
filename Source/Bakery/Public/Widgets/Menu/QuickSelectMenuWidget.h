// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSelectMenuWidget.generated.h"

class UQuickSelectMenuItemWidget;

UENUM(BlueprintType)
enum class EQuickSelectMenu : uint8
{
	None,
	CookingTools,
	RefrigeratorIngredients,
	ExtraIngredients
};

USTRUCT(BlueprintType)
struct FQuickSelectMenuItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="QuickSelectMenu|Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, Category = "QuickSelectMenu|Item")
	UTexture2D* ItemImage;
};

USTRUCT(BlueprintType)
struct FQuickSelectMenuData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "QuickSelectMenu")
	EQuickSelectMenu Menu;

	UPROPERTY(EditAnywhere, Category = "QuickSelectMenu")
	UTexture2D* ItemNameBackgroundImage;

	UPROPERTY(EditAnywhere, Category = "QuickSelectMenu")
	TArray<FQuickSelectMenuItem> Items;
};

/**
 * 
 */
UCLASS()
class BAKERY_API UQuickSelectMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(int InitalizeIndex);
	int Hide();

	void SetMenu(EQuickSelectMenu QuickSelectMenu);
	void SetFocus(int ItemIndex);

protected:
	void NativeConstruct() override;
	
private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* QuickSelectMenuAppearAnim;

	TMap<EQuickSelectMenu, const FQuickSelectMenuData*> MenuDataMap;
	TArray<UQuickSelectMenuItemWidget*> MenuItemWidgets;

	EQuickSelectMenu CurrentQuickSelectMenuType;
	const FQuickSelectMenuData* CurrentQuickMenuData;

	int CurrentFocusingItem = -1;
};
