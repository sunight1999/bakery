// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BakeryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ABakeryPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="GameOver")
	float RestartDelay = 5.f;

	FTimerHandle RestartTimer;
};
