// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDefines.generated.h"

/*
 * Interactable Component 태그
 */
#define INTERACTABLE_COMPONENT_REMODELABLE TEXT("remodel")
#define INTERACTABLE_COMPONENT_GRABBALE TEXT("grab")
#define INTERACTABLE_COMPONENT_KEEPABLE TEXT("keep")

 /*
  * Interaction 정보
  */
class UInteractorComponent;

USTRUCT(BlueprintType)
struct BAKERY_API FInteractionInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FInteractionInfo() {}
	FInteractionInfo(UInteractorComponent* Interactor, const FHitResult& HitResult)
		: Interactor(Interactor), HitResult(HitResult) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractorComponent* Interactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;
};
