// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IngredientData.generated.h"

enum class ECookingTool : uint8;
class URecipeData;
class UIngredientMeshData;

/**
 * 
 */
UCLASS()
class BAKERY_API UIngredientData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UIngredientData();

	bool IsPendingCooking() const { return bIsPendingCooking; }

	const FName& GetName() const { return Name; }
	const FName* GetCookingSoundTag(ECookingTool CookingTool) const { return CookingSoundTagMap.Find(CookingTool); }
	const UIngredientMeshData* GetIngredientMeshData() const { return IngredientMeshData; }

	const UIngredientData* CheckCookable(ECookingTool CookingTool) const
	{
		if (const UIngredientData* const* IngredientData = CookableToolMap.Find(CookingTool))
			return *IngredientData;
		
		return nullptr;
	}
	const UIngredientData* CheckMergeable(const UIngredientData* IngredientData) const
	{
		if (const UIngredientData* const* NewIngredientData = MergeableIngredientMap.Find(IngredientData))
			return *NewIngredientData;

		return nullptr;
	}

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta=(AllowPrivateAccess="true"))
	FName Name;
	
	// true일 경우, 조리가 완료되어도 데이터를 바로 바꾸지 않음
	// false일 경우, 조리가 완료되면 바로 조리된 재료로 데이터 변경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta = (AllowPrivateAccess = "true"))
	bool bIsPendingCooking = false;

	// 재료 메쉬 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta = (AllowPrivateAccess = "true"))
	UIngredientMeshData* IngredientMeshData;

	// 조리 방법별 효과음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta = (AllowPrivateAccess = "true"))
	TMap<ECookingTool, FName> CookingSoundTagMap;

	// 해당 재료를 조리(썰기, 찌기 등)해 만들 수 있는 재료
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta = (AllowPrivateAccess = "true"))
	TMap<ECookingTool, const UIngredientData*> CookableToolMap;

	// 해당 재료와 합칠 수 있는 재료
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta = (AllowPrivateAccess = "true"))
	TMap<const UIngredientData*, const UIngredientData*> MergeableIngredientMap;
};
