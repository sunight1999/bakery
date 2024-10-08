// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BakeryPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void ABakeryPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void ABakeryPlayerController::BeginPlay()
{
}
