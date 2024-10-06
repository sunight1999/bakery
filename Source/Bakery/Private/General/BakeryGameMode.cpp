// Copyright Epic Games, Inc. All Rights Reserved.

#include "General/BakeryGameMode.h"
#include "Characters/BakeryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "TimerManager.h"

#include "General/BakeryGameState.h"
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

	PlayerController = Cast<ABakeryPlayerController>(GetWorld()->GetFirstPlayerController());

	for (TActorIterator<ACustomerSpawner> It(GetWorld()); It; ++It)
	{
		CustomerSpawners.Add(*It);
	}
}

void ABakeryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (BakeryGameState->GetBakeryState() == EBakeryState::Opened)
	{
		CurrentOperatingTime += DeltaSeconds;
		PlayerController->GetBakeryHUD()->SetDayProgress(CurrentOperatingTime / OperatingTime);

		if (CurrentOperatingTime >= OperatingTime)
		{
			CloseBakery();
		}
	}
}

void ABakeryGameMode::OpenBakery()
{
	BakeryGameState->SetBakeryState(EBakeryState::Opened);
	PlayerController->GetBakeryHUD()->SetHUDState(true);
	CurrentOperatingTime = 0.f;

	for (ACustomerSpawner* Spawner : CustomerSpawners)
	{
		Spawner->Start();
	}
}

void ABakeryGameMode::CloseBakery()
{
	BakeryGameState->SetBakeryState(EBakeryState::Preparing);
	PlayerController->GetBakeryHUD()->SetHUDState(false);

	for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
	{
		(*It)->Leave();
	}

	for (ACustomerSpawner* Spawner : CustomerSpawners)
	{
		Spawner->Stop();
	}
}

