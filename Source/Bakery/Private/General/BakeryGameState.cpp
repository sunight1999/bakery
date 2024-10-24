// Fill out your copyright notice in the Description page of Project Settings.


#include "General/BakeryGameState.h"
#include "Kismet/GameplayStatics.h"

#include "General/BakerySaveGame.h"
#include "General/BakeryGameMode.h"

void ABakeryGameState::BeginPlay()
{
	LoadGame();

	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	BakeryGameMode->OnBakeryOpened.AddUObject(this, &ABakeryGameState::SetBakeryStateOpened);
	BakeryGameMode->OnBakeryOpened.AddUObject(this, &ABakeryGameState::AddDay);
	BakeryGameMode->OnBakeryClosed.AddUObject(this, &ABakeryGameState::SetBakeryStateClosed);
	BakeryGameMode->OnBakeryClosed.AddUObject(this, &ABakeryGameState::SaveGame);
}

void ABakeryGameState::LoadGame()
{
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(BAKERY_SAVEGAME_SLOT_NAME, 0);
	if (!SaveGame)
	{
		SaveGame = UGameplayStatics::CreateSaveGameObject(UBakerySaveGame::StaticClass());
	}

	UBakerySaveGame* BakerySaveGame = Cast<UBakerySaveGame>(SaveGame);
	AddMoney(BakerySaveGame->Money);
	AddDay(BakerySaveGame->Day);
}

void ABakeryGameState::SaveGame()
{
	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UBakerySaveGame::StaticClass());
	UBakerySaveGame* BakerySaveGame = Cast<UBakerySaveGame>(SaveGame);
	BakerySaveGame->Money = Money;
	BakerySaveGame->Day = Day;

	UE_LOG(LogTemp, Display, TEXT("GAME SAVED"));

	UGameplayStatics::SaveGameToSlot(BakerySaveGame, BAKERY_SAVEGAME_SLOT_NAME, 0);
}

void ABakeryGameState::AddMoney(int32 InMoney)
{
	Money += InMoney;

	OnMoneyChanged.Broadcast(Money);
}

void ABakeryGameState::AddDay(int32 InDay)
{
	Day += InDay;

	OnDayChanged.Broadcast(Day);
}

void ABakeryGameState::AddDay()
{
	AddDay(1);
}