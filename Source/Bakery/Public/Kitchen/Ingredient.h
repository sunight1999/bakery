// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

class URecipeSubsystem;
class UIngredientData;
class URecipeData;

UCLASS()
class BAKERY_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	AIngredient();

	void ChangeIngredient(const UIngredientData* NewIngredientData);
	const UIngredientData* MergeIngredient(const AIngredient* OtherIngredient);

	const UIngredientData* GetIngredientData() const { return IngredientData; };
	const URecipeData* GetAvailableRecipe() const { return CurrentAvailableRecipe; }

protected:
	virtual void BeginPlay() override;

private:
	const URecipeSubsystem* RecipeSubsystem;

	const UIngredientData* IngredientData;
	UStaticMeshComponent* IngredientMesh;

	const URecipeData* CurrentAvailableRecipe;
};
