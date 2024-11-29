// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Menu/QuickSelectMenuWidget.h"
#include "UObject/ConstructorHelpers.h"

#include "Widgets/Menu/QuickSelectMenuItemWidget.h"

void UQuickSelectMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 퀵 메뉴 구성 정보 로딩
	//ConstructorHelpers::FObjectFinder<UDataTable> QuickMenuDataTableFinder();
	//if (QuickMenuDataTableFinder.Succeeded())
	//{
	if (UObject* Object = StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/Data/DT_QuickMenu")))
	{
		if (UDataTable* MenuDataTable = Cast<UDataTable>(Object))
		{
			TArray<const FQuickSelectMenuData*> MenuDatas;
			MenuDataTable->GetAllRows(FString("QuickSelectMenu"), MenuDatas);

			for (auto MenuData : MenuDatas)
			{
				MenuDataMap.Emplace(MenuData->Menu, MenuData);
			}
		}
	}

	// 퀵 메뉴 아이템 위젯 로딩
	for (int32 i = 0; i < 4; i++)
	{
		FString MenuItemName = FString::Printf(TEXT("QuickSelectMenuItem_%d"), i);
		UQuickSelectMenuItemWidget* MenuItem = Cast<UQuickSelectMenuItemWidget>(GetWidgetFromName(FName(MenuItemName)));
		MenuItemWidgets.Emplace(MenuItem);
	}

	verify(MenuItemWidgets.Num() > 0)
}

void UQuickSelectMenuWidget::Show(int InitalizeIndex)
{
	QueuePlayAnimation(QuickSelectMenuAppearAnim);
	CurrentFocusingItem = -1;
	SetFocus(InitalizeIndex);
}

int UQuickSelectMenuWidget::Hide()
{
	QueuePlayAnimationReverse(QuickSelectMenuAppearAnim);
	MenuItemWidgets[CurrentFocusingItem]->SetFocus(false);
	return CurrentFocusingItem;
}

void UQuickSelectMenuWidget::SetMenu(EQuickSelectMenu QuickSelectMenu)
{
	if (CurrentQuickSelectMenuType == QuickSelectMenu)
	{
		return;
	}

	CurrentQuickSelectMenuType = QuickSelectMenu;

	const FQuickSelectMenuData* const* QuickSelecetMenuData = MenuDataMap.Find(QuickSelectMenu);
	if (!QuickSelecetMenuData)
	{
		UE_LOG(LogTemp, Warning, TEXT("퀵 메뉴 구성 정보를 찾을 수 없습니다."));
		return;
	}

	CurrentQuickMenuData = *QuickSelecetMenuData;
	for (int i = 0; i < MenuItemWidgets.Num(); i++)
	{
		auto MenuItemWidget = MenuItemWidgets[i];
		if (CurrentQuickMenuData->Items.Num() <= i)
		{
			break;
		}

		auto MenuItemData = CurrentQuickMenuData->Items[i];

		// 선택 항목 정보 갱신
		MenuItemWidget->SetItemNameBackgroundImage(CurrentQuickMenuData->ItemNameBackgroundImage);
		MenuItemWidget->SetItemImage(MenuItemData.ItemImage);
		MenuItemWidget->SetItemName(MenuItemData.ItemName);
	}
}

void UQuickSelectMenuWidget::SetFocus(int ItemIndex)
{
	if (CurrentFocusingItem == ItemIndex)
	{
		return;
	}

	if (CurrentFocusingItem != -1)
	{
		MenuItemWidgets[CurrentFocusingItem]->SetFocus(false);
	}

	CurrentFocusingItem = ItemIndex;
	MenuItemWidgets[CurrentFocusingItem]->SetFocus(true);
}
