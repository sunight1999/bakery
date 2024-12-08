// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "SprinklerAbnormalityEvent.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ASprinklerAbnormalityEvent : public AAbnormalityEvent
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Cause() override;
	virtual void Finish() override;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="AbnormalityEvent|Sprinkler")
	TSubclassOf<AActor> SprinklerActorClass;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Sprinkler")
	FVector SprinklerSpawnLocation;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Sprinkler")
	int SpawnRowNum = 2;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Sprinkler")
	int SpawnColumnNum = 3;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Sprinkler")
	float SpawnRowDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Sprinkler")
	float SpawnColumnDistance = 600.f;

	TArray<AActor*> Sprinklers;
};
