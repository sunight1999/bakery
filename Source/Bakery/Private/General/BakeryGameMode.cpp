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
}

void ABakeryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (BakeryGameState->GetBakeryState() == EBakeryState::Opened)
	{
		CurrentOperatingTime += DeltaSeconds;
		BakeryHUDWidget->SetDayProgress(CurrentOperatingTime / OperatingTime);

		if (CurrentOperatingTime >= OperatingTime)
		{
			CloseBakery();
		}
	}
}

void ABakeryGameMode::OpenBakery()
{
	BakeryGameState->SetBakeryState(EBakeryState::Opened);
	BakeryHUDWidget->SetHUDState(true);
	CurrentOperatingTime = 0.f;

	for (ACustomerSpawner* Spawner : CustomerSpawners)
	{
		Spawner->Start();
	}
}

void ABakeryGameMode::CloseBakery()
{
	BakeryGameState->SetBakeryState(EBakeryState::Preparing);
	BakeryHUDWidget->SetHUDState(false);

	for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
	{
		(*It)->Leave();
	}

	for (ACustomerSpawner* Spawner : CustomerSpawners)
	{
		Spawner->Stop();
	}
}

