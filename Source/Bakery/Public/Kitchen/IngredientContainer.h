// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "IngredientContainer.generated.h"

class UIngredientData;

/**
 * 
 */
UCLASS()
class BAKERY_API AIngredientContainer : public AGrabInteractableActor
{
	GENERATED_BODY()

public:
	AIngredientContainer();
	void Tick(float DeltaTime) override;

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override {}
	void OnInteract() override {}
	void OnExitInteract() override {}

	void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	void OnGrab() override {}
	void OnExitGrab() override {}

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Kitchen|Ingredient")
	UStaticMeshComponent* ContainerMesh;

	UPROPERTY(EditAnywhere, Category="Kitchen|Ingredient")
	TObjectPtr<UIngredientData> ContainedIngredient;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Ingredient")
	float IngredientMeshSizeMultiplier = 1.f;

	UWorld* World;

	/* TODO: 풀링으로 변경 시 활성화
	UPROPERTY(EditAnywhere, Category = "Kitchen|Ingredient")
	uint8 PoolingInitNum = 10;	// Ingredient 액터 풀링 초기 개수

	UPROPERTY(EditAnywhere, Category = "Kitchen|Ingredient")
	uint8 PoolingStepNum = 10;	// Ingredient 액터 풀의 액터를 모두 사용 중일 경우 풀링 개수를 늘릴 단위

	uint8 MaxPoolingNum = 0;	// 현재 풀링 개수
	uint8 UsingPoolingNum = 0;	// 현재 풀에서 사용 중인 액터 수
	*/
};
