// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Ingredient.h"
#include "UObject/ObjectSaveContext.h"

#include "Subsystems/RecipeSubsystem.h"
#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Data/IngredientData.h"
#include "Kitchen/Data/RecipeData.h"

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = false;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientMesh"));
	RootComponent = IngredientMesh;

	// Collision 설정
	IngredientMesh->SetMobility(EComponentMobility::Movable);
	IngredientMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	IngredientMesh->SetCollisionProfileName(FName("Ingredient"));

	// Physics 설정
	IngredientMesh->SetSimulatePhysics(true);
	IngredientMesh->SetEnableGravity(false);
}

void AIngredient::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	RecipeSubsystem = GameInstance->GetSubsystem<URecipeSubsystem>();

	verify(RecipeSubsystem);
}

/// <summary>
/// 다른 재료로 변경합니다.
/// 변경된 재료가 레시피에 등록되어 있을 경우 해당 레시피 정보를 가져옵니다.
/// </summary>
/// <param name="TargetIngredientData">변경할 재료</param>
void AIngredient::ChangeIngredient(const UIngredientData* NewIngredientData)
{
	verify(NewIngredientData);

	IngredientData = NewIngredientData;
	IngredientMesh->SetStaticMesh(IngredientData->GetIngredientMesh());
	CurrentAvailableRecipe = RecipeSubsystem->GetRecipe(NewIngredientData->GetName());
}

/// <summary>
/// 재료를 다른 재료와 합칩니다.
/// 두 재료 중 레시피 정보를 가지고 있는 재료가 있으면 해당 레시피를 통해 재료를 합쳤을 때 나올 요리를 유추합니다.
/// 두 재료 모두 레시피 정보가 없을 경우 Root 레시피를 통해 다음 요리를 유추합니다.
/// </summary>
/// <param name="OtherIngredient">합칠 재료</param>
/// <returns>두 재료를 사용하는 레시피의 결과물</returns>
const UIngredientData* AIngredient::MergeIngredient(const AIngredient* OtherIngredient)
{
	// 두 재료 중 유효한 Recipe를 할당. 만약 둘 다 nullptr라면 Root 레시피를 가져와 사용
	const URecipeData* AvailableRecipe = GetAvailableRecipe();
	if (!AvailableRecipe)
	{
		AvailableRecipe = OtherIngredient->GetAvailableRecipe();
		if (!AvailableRecipe)
		{
			AvailableRecipe = RecipeSubsystem->GetRecipeRoot();
		}
	}

	// 두 재료를 사용하는 레시피가 있을 경우 해당 레시피의 결과물(UIngredientData)을 반환
	const TSet<URecipeData*>& NextRecipes = AvailableRecipe->GetNextRecipes();
	for (URecipeData* NextRecipe : NextRecipes)
	{
		bool bIsThisIncluded = NextRecipe->Includes(IngredientData);
		bool bIsOtherIncluded = NextRecipe->Includes(OtherIngredient->GetIngredientData());
		if (bIsThisIncluded && bIsOtherIncluded)
		{
			return NextRecipe->GetResult();
		}
	}

	return nullptr;
}

