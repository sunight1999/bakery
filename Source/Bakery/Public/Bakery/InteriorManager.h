// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteriorManager.generated.h"

class ACloner;

UCLASS()
class BAKERY_API AInteriorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteriorManager();
	void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Bakery")
	ACloner* BakeryFloors;

	UPROPERTY(EditAnywhere, Category = "Bakery")
	ACloner* BakeryWalls[3];

	UPROPERTY(EditAnywhere, Category = "Bakery")
	AActor* BakeryCornerWalls[2];
};
