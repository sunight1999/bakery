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
	UPROPERTY(EditAnywhere, Category = "Data")
	FString Situation;
	UPROPERTY(EditAnywhere, Category = "Data")
	FString Explanation;
	UPROPERTY(EditAnywhere, Category = "Data")
	FString Other;
};