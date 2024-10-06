// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BakeryPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

#include "Widgets/HUD/BakeryHUDWidget.h"

void ABakeryPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

// TODO: UI 생성 및 관리는 UISubsystem으로 옮길 예정
void ABakeryPlayerController::BeginPlay()
{
	BakeryHUDWidget = Cast<UBakeryHUDWidget>(CreateWidget(GetWorld(), BakeryHUDWidgetClass));
	if (BakeryHUDWidget)
	{
		BakeryHUDWidget->AddToViewport();
	}
}
