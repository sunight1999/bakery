// Copyright Epic Games, Inc. All Rights Reserved.

#include "General/BakeryGameMode.h"
#include "Characters/BakeryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "TimerManager.h"

#include "General/BakeryGameState.h"
#include "Subsystems/UISubsystem.h"
#include "Characters/BakeryPlayerController.h"
#include "Characters/Customer.h"
#include "Widgets/HUD/BakeryHUDWidget.h"
#include "General/Tools/CustomerSpawner.h"

ABakeryGameMode::ABakeryGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABakeryGameMode::BeginPlay()
{
	Super::BeginPlay();

	BakeryGameState = Cast<ABakeryGameState>(GameState);
	UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	UISubsystem->LoadAllUI();

	UUserWidget* HUDWidget = UISubsystem->SetUIVisibility(FName("BakeryHUD"), ESlateVisibility::SelfHitTestInvisible);
	BakeryHUDWidget = Cast<UBakeryHUDWidget>(HUDWidget);

	PlayerController = Cast<ABakeryPlayerController>(GetWorld()->GetFirstPlayerController());

	for (TActorIterator<ACustomerSpawner> It(GetWorld()); It; ++It)
	{
		CustomerSpawners.Add(*It);
	}

	float TimeTickRate = 1.f / GameTimeMultiplier;
	BakeryGameState->SetTime(GameStartTime);
	GetWorldTimerManager().SetTimer(GameTimeHandle, BakeryGameState, &ABakeryGameState::AddTime, TimeTickRate, true, TimeTickRate);
}

void ABakeryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (BakeryGameState->GetBakeryState() == EBakeryState::Opened)
	{
		if (BakeryGameState->GetElapsedTime() >= BakeryCloseTime)
		{
			CloseBakery();
		}
	}
	else if (BakeryGameState->GetBakeryState() == EBakeryState::Closed)
	{
		if (BakeryGameState->GetElapsedTime() >= BakeryOpenTime)
		{
			OpenBakery();
		}
	}
}

void ABakeryGameMode::OpenBakery()
{
	BakeryHUDWidget->SetHUDState(true);

	BakeryGameState->SetTime(BakeryOpenTime);

	OnBakeryPreOpened.Broadcast();
	OnBakeryOpened.Broadcast();
}

void ABakeryGameMode::CloseBakery()
{
	BakeryHUDWidget->SetHUDState(false);

	BakeryGameState->SetTime(BakeryCloseTime);

	for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
	{
		(*It)->Leave();
	}

	OnBakeryClosed.Broadcast();
}

bool ABakeryGameMode::IsOpened()
{
	return BakeryHUDWidget->GetHUDState();
}
