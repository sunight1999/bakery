// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SituationDatabase.generated.h"

/**
 *  상황 보고 시스템을 위한 데이터베이스 구조체
 */

USTRUCT(BlueprintType)
struct FSituationData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSituationData(){}

	UPROPERTY(EditAnywhere, Category = "Data")
	FString Situation;
};
