// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/RecipeSubsystem.h"
#include "Engine/AssetManager.h"

#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Data/RecipeData.h"

URecipeSubsystem::URecipeSubsystem()
{
}

void URecipeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Load(KITCHEN_ASSETTYPE_RECIPE);
}

void URecipeSubsystem::Load(const FPrimaryAssetType& AssetType)
{
	UAssetManager& AssetManager = UAssetManager::Get();

	// AssetType으로 레시피 에셋 ID 검색
	TArray<FPrimaryAssetId> RecipeAssetIds;
	if (!AssetManager.GetPrimaryAssetIdList(AssetType, RecipeAssetIds))
	{
		UE_LOG(LogTemp, Warning, TEXT("Recipe 관련 에셋을 찾을 수 없습니다."));
		return;
	}

	// 검색된 ID로 해당 에셋 데이터를 실제로 로딩
	// TODO: 비동기 로딩으로 변경
	for (const FPrimaryAssetId& Id : RecipeAssetIds)
	{
		FSoftObjectPtr RecipeObjectPtr(AssetManager.GetPrimaryAssetPath(Id));
		
		if (RecipeObjectPtr.IsPending())
		{
			RecipeObjectPtr.LoadSynchronous();
		}

		URecipeData* RecipeData = CastChecked<URecipeData>(RecipeObjectPtr.Get());
		RecipeMap.Add(Id.PrimaryAssetName, RecipeData);

		if (!RecipeData)
		{
			UE_LOG(LogTemp, Warning, TEXT("Recipe DataAsset '%s' is null"), *Id.PrimaryAssetName.ToString());
		}
	}

	RecipeRoot = *RecipeMap.Find("Root");
}

const URecipeData* URecipeSubsystem::GetRecipe(FName Name) const
{
	const URecipeData* const* RecipeDataPtr = RecipeMap.Find(Name);
	if (RecipeDataPtr)
	{
		return *RecipeDataPtr;
	}
	else
	{
		return nullptr;
	}
}

const URecipeData* URecipeSubsystem::GetRecipeRoot() const
{
	return RecipeRoot;
}
