// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RecipeSubsystem.generated.h"

class URecipeData;

/**
 * 
 */
UCLASS()
class BAKERY_API URecipeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	URecipeSubsystem();
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Load(const FPrimaryAssetType& AssetType);

	const URecipeData* GetRecipe(FName Name) const;
	const URecipeData* GetRecipeRoot() const;

private:
	TMap<FName, const URecipeData*> RecipeMap;
	const URecipeData* RecipeRoot;
};
