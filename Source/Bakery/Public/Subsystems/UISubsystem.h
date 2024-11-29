// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/SlateWrapperTypes.h"
#include "UISubsystem.generated.h"

#define UI_LAYER_ZORDER_OFFSET 1000
#define UI_UNASSIGNED_LAYER -2147484

UENUM(BlueprintType)
enum class EUICategory : uint8
{
	Dialogue,
	HUD,
	Menu
};

UENUM(BlueprintType)
enum class EUILayerRule : uint8
{
	Root,
	Additive,
	Exclusive
};

USTRUCT(BlueprintType)
struct FUIInformation : public FTableRowBase
{
	GENERATED_BODY()

public:
	FORCEINLINE int GetZOrder() const { return bIsHUD ? ZOrder : Layer * UI_LAYER_ZORDER_OFFSET + ZOrder; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> UIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUICategory Category;

	/*
	 * 해당 Layer에서 UI가 맡은 역할
	 * Root: 기본 UI. Additive나 Exclusive를 띄우려면 반드시 먼저 표시해야 함
	 * Additive: Root에서 연계될 수 있는 UI. ZOrder에 따라 차곡차곡 겹쳐짐
	 * Exclusive: Root에서 연계될 수 있는 UI. 여러 개의 Exclusive가 정의될 수 있지만 Viewport에는 하나만 표시됨 (인벤토리, 장비 등 모든 메뉴가 같은 UI에 있는 경우 등)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUILayerRule LayerRule;

	/*
	 * 일종의 UI 그룹
	 * UI의 Z Order는 레이어를 기준으로 먼저 정렬하고, 멤버 변수 ZOrder에 따라 레이어 내부에서 다시 정렬함
	 * 이를 위해 AddToViewPort() 호출 시 실제로 설정되는 Z Order는 (Layer * UI_LAYER_ZORDER_OFFSET + ZOrder)
	 */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="-2147483", UIMax="2147482", ClampMin = "-2147483", ClampMax = "2147482"))
	int32 Layer;

	// Layer 내에서의 Z Order
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0", UIMax = "999", ClampMin = "0", ClampMax = "999"))
	int32 ZOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility DefaultVisibility = ESlateVisibility::Collapsed;

	// HUD 여부. HUD는 Layer에 영향을 받지 않고 ZOrder를 그대로 사용함
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHUD;
};

USTRUCT()
struct FManagedUI
{
	GENERATED_BODY()

public:
	FManagedUI() {}
	FManagedUI(FUIInformation* InUIManagementInfo, UUserWidget* InWidget) :
		UIManagementInfo(InUIManagementInfo), Widget(InWidget) {}

	const FUIInformation* UIManagementInfo = nullptr;
	UUserWidget* Widget = nullptr;
};

/**
 * 
 */
UCLASS()
class BAKERY_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UUISubsystem();
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadAllUI();
	bool IsWorldValid();

	UUserWidget* GetUIObject(FName UIName);
	UUserWidget* SetUIVisibility(FName UIName, ESlateVisibility Visibility);
	void RevertUILayer();

private:
	bool AddLayerStack(FManagedUI* ManagedUI);
	void RemoveLayerStack(FManagedUI* ManagedUI);

	UPROPERTY(EditAnywhere)
	UDataTable* UIDataTable;
	TMap<FName, FManagedUI> WidgetMap;

	UGameInstance* GameInstance;
	UWorld* World;

	int32 CurrentLayer = UI_UNASSIGNED_LAYER;
	TMap<int32, TArray<FManagedUI*>> LayerStackMap;
};
