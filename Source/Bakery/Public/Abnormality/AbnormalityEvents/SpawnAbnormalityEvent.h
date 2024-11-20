// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "SpawnAbnormalityEvent.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ASpawnAbnormalityEvent : public AAbnormalityEvent
{
	GENERATED_BODY()
	
public:
	virtual void Cause() override;
};
