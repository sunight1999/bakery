// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDefines.generated.h"

 /*
  * Interaction 정보
  */
class UInteractorComponent;

USTRUCT(BlueprintType)
struct BAKERY_API FInteractionInfo
{
	GENERATED_BODY()

public:
	FInteractionInfo() { Interactor = nullptr; }
	FInteractionInfo(UInteractorComponent* Interactor, const FHitResult& HitResult)
		: Interactor(Interactor), HitResult(HitResult) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractorComponent* Interactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;

	static const FInteractionInfo NoneInteraction;
};
