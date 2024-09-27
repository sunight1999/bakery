// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BakeryManager.generated.h"

UCLASS()
class BAKERY_API ABakeryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABakeryManager();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	
};
