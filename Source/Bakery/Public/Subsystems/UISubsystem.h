// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

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
	Set,
	Add
};

USTRUCT(BlueprintType)
struct FManagedUI : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> UIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUICategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUILayerRule LayerRule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ZOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHUD;
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

	void SetUIVisibility(FName UIName, ESlateVisibility Visibility);
	void ReverseUILayer();

private:
	UPROPERTY(EditAnywhere)
	UDataTable* UIDataTable;

	int CurrentLayer;
	TArray<UUserWidget*> LayerStack;
};
