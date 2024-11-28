// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

enum class ECookingTool : uint8;
class UBoxComponent;
class URecipeSubsystem;
class UIngredientData;
class UIngredientMeshData;
class URecipeData;
class UNiagaraComponent;

UCLASS()
class BAKERY_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	AIngredient();
	void InitializeMeshComponent(UMeshComponent* MeshComponent);

	void PlayBeingCookedAnimation(ECookingTool CookingTool);
	void StopBeingCookedAnimation();

	void ChangeIngredient(const UIngredientData* NewIngredientData, bool bIsRaw = false);
	const UIngredientData* TryMergeIngredient(const AIngredient* OtherIngredient);

	const UIngredientData* GetIngredientData() const { return IngredientData; };
	const URecipeData* GetSourceRecipe() const { return SourceRecipe; }

	bool IsCooked() const { return bIsCooked; }
	void SetIsCooked(bool bCooked) { bIsCooked = bCooked; }
	void CompletePendingCooking(ECookingTool CookingTool);

protected:
	virtual void BeginPlay() override;

private:
	void Clear();
	void SetIngredientMesh();

	const URecipeSubsystem* RecipeSubsystem;

	/*
	 * 재료 데이터 관련
	 */
	// 재료가 bIsPendingCooking = true일 경우, 조리가 끝났도 재료 데이터 정보가 바로 바뀌지 않음
	// 이후 조리대, 접시, 도마 등에 놓아야 다음 재료 데이터 정보로 바뀜
	bool bIsCooked = false;

	const UIngredientData* IngredientData;
	const URecipeData* SourceRecipe;

	/*
	 * 컴포넌트
	 */
	UBoxComponent* RootBoxComponent;
	UNiagaraComponent* CookingEffect;

	UStaticMeshComponent* IngredientBottomStaticMesh;
	UStaticMeshComponent* IngredientBodyStaticMesh;
	UStaticMeshComponent* IngredientLidStaticMesh;

	USkeletalMeshComponent* IngredientBottomSkeletalMesh;
	USkeletalMeshComponent* IngredientBodySkeletalMesh;
	USkeletalMeshComponent* IngredientLidSkeletalMesh;
};
