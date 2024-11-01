// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IngredientData.generated.h"

enum class ECookingTool : uint8;

/**
 * 
 */
UCLASS()
class BAKERY_API UIngredientData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UIngredientData();
	void PreSave(FObjectPreSaveContext SaveContext) override;

	/*
	 * Getter
	 */
	const FName& GetName() const { return Name; }
	const FName* GetCookingSoundTag(ECookingTool CookingTool) const { return CookingSoundTagMap.Find(CookingTool); }
	UStaticMesh* GetIngredientMesh() const { return IngredientMesh; }

	const UIngredientData* CheckCookable(ECookingTool CookingTool) const
	{
		const UIngredientData* const* CookedIngredientPtr = CookedIngredientMap.Find(CookingTool);

		if (CookedIngredientPtr)
		{
			return *CookedIngredientPtr;
		}

		return nullptr;
	}

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Ingredient", meta=(AllowPrivateAccess="true"))
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Ingredient", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* IngredientMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Ingredient", meta = (AllowPrivateAccess = "true"))
	TMap<ECookingTool, FName> CookingSoundTagMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitchen|Ingredient", meta = (AllowPrivateAccess = "true"))
	TMap<ECookingTool, const UIngredientData*> CookedIngredientMap;
};
