// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../KitchenDefines.h"
#include "RecipeData.generated.h"

class UIngredientData;

/**
 * 
 */
UCLASS()
class BAKERY_API URecipeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URecipeData();
	void PreSave(FObjectPreSaveContext SaveContext) override;

	FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId(AssetType, GetFName()); }
	const FName GetName() const { return Name; }
	const TSet<URecipeData*>& GetNextRecipes() const { return NextRecipes; }
	const UIngredientData* GetResult() const { return ItselfResult; }
	int GetPrice() const { return Price; }

	bool Includes(const UIngredientData* Ingredient) const { return Ingredients.Contains(Ingredient); }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	FPrimaryAssetType AssetType = KITCHEN_ASSETTYPE_RECIPE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	TSet<UIngredientData*> Ingredients;	// 이 레시피의 요리를 제작하기 위해 필요한 재료

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	int32 Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	TSet<URecipeData*> PrevRecipes;	// 이 레시피를 해금하기 위해 필요한 하위 레시피

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	TSet<URecipeData*> NextRecipes;	// 이 레시피 다음으로 해금할 수 있는 상위 레시피

	// 해당 레시피로 만들어지는 요리(IngredientData)로, 재료를 합칠 때 새로 설정할 IngredientData를 구하고자 사용됨
	// 예) 백설기 레시피인 경우 백설기 IngredientData를 저장
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Recipe", meta = (AllowPrivateAccess = "true"))
	UIngredientData* ItselfResult;
};
