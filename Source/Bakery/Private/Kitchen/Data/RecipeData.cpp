// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Data/RecipeData.h"
#include "UObject/ObjectSaveContext.h"
#include "Engine/AssetManager.h"

#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Data/IngredientData.h"

URecipeData::URecipeData()
{

}

void URecipeData::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 재료 이름을 에셋의 파일명으로 지정
	Name = GetFName();
}

const UIngredientData* URecipeData::GetResult()
{
	if (!ItselfResult)
	{
		UAssetManager& AssetManager = UAssetManager::Get();

		// 레시피 Output IngredientData를 에셋명으로 찾아 등록
		FPrimaryAssetId AssetId(KITCHEN_ASSETTYPE_INGREDIENT, Name);
		FSoftObjectPtr IngredientObjectPtr(AssetManager.GetPrimaryAssetPath(AssetId));

		if (IngredientObjectPtr.IsPending())
		{
			IngredientObjectPtr.LoadSynchronous();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s 이름으로 된 Ingredient 에셋이 존재하지 않습니다."), *Name.ToString());
			return nullptr;
		}

		UIngredientData* IngredientData = CastChecked<UIngredientData>(IngredientObjectPtr.Get());
		if (!IngredientData)
		{
			UE_LOG(LogTemp, Error, TEXT("%s 이름으로 된 Ingredient 에셋이 존재하지 않습니다."), *Name.ToString());
			return nullptr;
		}

		ItselfResult = IngredientData;
	}

	return ItselfResult;
}
