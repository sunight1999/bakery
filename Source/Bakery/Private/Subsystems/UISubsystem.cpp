// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UISubsystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UUISubsystem::UUISubsystem()
{
	ConstructorHelpers::FObjectFinder<UDataTable> UIDataTableFinder(TEXT("/Game/Data/DT_ManagedUI"));
	if (UIDataTableFinder.Succeeded())
	{
		UIDataTable = UIDataTableFinder.Object;
	}

	verify(UIDataTable);
}

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GameInstance = GetGameInstance();
	if (!IsWorldValid())
	{
		return;
	}
}

void UUISubsystem::LoadAllUI()
{
	if (!IsWorldValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("World가 유효하지 않습니다."));
		return;
	}

	// DT_ManagedUI에 정의된 데이터에 따라 Widget 초기화
	FString ContextString = "ManagedUI";
	TArray<FName> ManagedUINames = UIDataTable->GetRowNames();
	for (const FName& UIName : ManagedUINames)
	{
		FUIInformation* UIInformation = UIDataTable->FindRow<FUIInformation>(UIName, ContextString);
		if (!UIInformation->UIClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s의 Widget Class가 설정되지 않았습니다."), *UIName.ToString())
			continue;
		}

		UUserWidget* Widget = CreateWidget<UUserWidget>(World, UIInformation->UIClass);

		Widget->AddToViewport(UIInformation->GetZOrder());
		Widget->SetVisibility(UIInformation->DefaultVisibility);
		WidgetMap.Emplace(UIName, FManagedUI(UIInformation, Widget));
	}
}

bool UUISubsystem::IsWorldValid()
{
	if (!World || !IsValid(World))
	{
		World = GameInstance->GetWorld();
	}
	
	return IsValid(World);
}

UUserWidget* UUISubsystem::GetUIObject(FName UIName)
{
	if (!IsWorldValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("World가 유효하지 않습니다."));
		return nullptr;
	}

	FManagedUI* ManagedUI = WidgetMap.Find(UIName);
	if (!ManagedUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("UI 이름(%s)이 유효하지 않습니다."), *UIName.ToString());
		return nullptr;
	}

	const FUIInformation* UIInformation = ManagedUI->UIManagementInfo;
	return ManagedUI->Widget;
}

UUserWidget* UUISubsystem::SetUIVisibility(FName UIName, ESlateVisibility Visibility)
{
	if (!IsWorldValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("World가 유효하지 않습니다."));
		return nullptr;
	}

	FManagedUI* ManagedUI = WidgetMap.Find(UIName);
	if (!ManagedUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("UI 이름(%s)이 유효하지 않습니다."), *UIName.ToString());
		return nullptr;
	}

	const FUIInformation* UIInformation = ManagedUI->UIManagementInfo;
	UUserWidget* Widget = ManagedUI->Widget;

	// HUD의 경우 UI 레이어와 독립적으로 처리됨
	if (UIInformation->bIsHUD)
	{
		Widget->SetVisibility(Visibility);
		return Widget;
	}

	// LayerStack 관리
	if (Visibility == ESlateVisibility::Collapsed || Visibility == ESlateVisibility::Hidden)
	{
		Widget->SetVisibility(Visibility);
		RemoveLayerStack(ManagedUI);
	}
	else
	{
		// Root를 띄우지 않고 Additive나 Exclusive를 띄우려는 경우 LayerStack에 추가할 수 없으므로 결과 확인 선행
		if (AddLayerStack(ManagedUI))
		{
			Widget->SetVisibility(Visibility);
		}
	}

	return Widget;
}

// add stack에서 하나씩 pop하며 invisible 처리
	// CurrentLayer 먼저 처리
	// CurrentLayer가 유효하지 않으면 가장 높은 Layer를 CurrentLayer로 처리

/// <summary>
/// 가장 최근에 띄운 UI부터 차례대로 비가시화. 최근에 띄운 UI의 Layer, 높은 Layer 순으로 처리됨
/// </summary>
void UUISubsystem::RevertUILayer()
{
	if (LayerStackMap.Num() == 0)
	{
		return;
	}

	if (CurrentLayer == UI_UNASSIGNED_LAYER)
	{
		TArray<int32> StackedLayers;
		LayerStackMap.GetKeys(StackedLayers);
		StackedLayers.Sort();

		CurrentLayer = StackedLayers.Last();
	}

	TArray<FManagedUI*>* LayerStack = LayerStackMap.Find(CurrentLayer);
	FManagedUI* TargetUI = LayerStack->Last();
	
	TargetUI->Widget->SetVisibility(ESlateVisibility::Collapsed);
	LayerStack->RemoveAt(LayerStack->Num() - 1);

	if (LayerStack->Num() == 0)
	{
		LayerStackMap.Remove(CurrentLayer);
		CurrentLayer = UI_UNASSIGNED_LAYER;
	}
}

bool UUISubsystem::AddLayerStack(FManagedUI* ManagedUI)
{
	const FUIInformation* UIInformation = ManagedUI->UIManagementInfo;
	TArray<FManagedUI*>* LayerStack = LayerStackMap.Find(UIInformation->Layer);

	if (UIInformation->LayerRule == EUILayerRule::Root)
	{
		// 한 레이어에는 하나의 Root만 존재할 수 있으므로 만약 해당 레이어에 이미 Root가 띄워져 있다면 레이어 초기화
		if (LayerStack)
		{
			RemoveLayerStack(ManagedUI);
		}

		TArray<FManagedUI*> NewStack;
		NewStack.Emplace(ManagedUI);
		LayerStackMap.Emplace(UIInformation->Layer, NewStack);

		CurrentLayer = UIInformation->Layer;
		return true;
	}
	
	// Root UI를 띄우기 전에 Additive나 Exclusive를 띄우려고 한 경우. Root를 먼저 띄어야하므로 false 리턴
	if (!LayerStack)
	{
		return false;
	}

	// LayerStack에 UI 추가
	if (UIInformation->LayerRule == EUILayerRule::Additive)
	{
		// 만약 이미 LayerStack에 존재하는 UI라면 위치만 제일 위(마지막)로 이동시키기 위해 RemoveAt 수행
		for (int i = 0; i < LayerStack->Num(); i++)
		{
			if ((*LayerStack)[i] != ManagedUI)
			{
				continue;
			}
			
			// 마지막에 위치한 경우 이동시킬 필요가 없으므로 return
			if (i + 1 == LayerStack->Num())
			{
				return true;
			}

			LayerStack->RemoveAt(i);
			break;
		}

		LayerStack->Add(ManagedUI);
	}
	// LayerStack에 이미 존재하는 Exclusive UI를 제거하고 UI 추가
	else
	{
		for (int i = 0; i < LayerStack->Num(); i++)
		{
			FManagedUI* TargetUI = (*LayerStack)[i];
			if (TargetUI->UIManagementInfo->LayerRule == EUILayerRule::Exclusive)
			{
				TargetUI->Widget->SetVisibility(ESlateVisibility::Collapsed);
				LayerStack->RemoveAt(i);
				break;
			}
		}

		LayerStack->Emplace(ManagedUI);
	}

	CurrentLayer = UIInformation->Layer;
	return true;
}

void UUISubsystem::RemoveLayerStack(FManagedUI* TargetUI)
{
	const FUIInformation* UIInformation = TargetUI->UIManagementInfo;
	TArray<FManagedUI*>* LayerStack = LayerStackMap.Find(UIInformation->Layer);

	if (!LayerStack)
	{
		return;
	}

	// Root일 경우 속해 있는 레이어의 모든 UI를 비가시화하고 LayerStackMap에서 해당 LayerStack을 제거
	if (UIInformation->LayerRule == EUILayerRule::Root)
	{
		for (FManagedUI* StackedUI : *LayerStack)
		{
			StackedUI->Widget->SetVisibility(ESlateVisibility::Collapsed);
		}

		LayerStack->Empty();
		LayerStackMap.Remove(UIInformation->Layer);

		if (CurrentLayer == UIInformation->Layer)
		{
			CurrentLayer = UI_UNASSIGNED_LAYER;
		}

		return;
	}
	// Root가 아닐 경우 해당 UI만 LayerStack에서 제거
	else
	{
		for (int i = 0; i < LayerStack->Num(); i++)
		{
			FManagedUI* StackedUI = (*LayerStack)[i];

			if (TargetUI == StackedUI)
			{
				LayerStack->RemoveAt(i);
				return;
			}
		}
	}
}
