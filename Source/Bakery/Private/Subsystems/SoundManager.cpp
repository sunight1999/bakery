// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SoundManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoundManager* USoundManager::Instance = nullptr;

USoundManager::USoundManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> SoundDataTableFinder(TEXT("/Game/Data/DT_Sound"));
	if (SoundDataTableFinder.Succeeded())
	{
		SoundDataTable = SoundDataTableFinder.Object;
	}
}

USoundManager::~USoundManager()
{
	Instance = nullptr;
}

USoundManager* USoundManager::GetInstance(UWorld* World)
{
	if (!Instance)
	{
		Instance = NewObject<USoundManager>(World);
		Instance->AudioPool.OnPostSpawned.BindUObject(Instance, &USoundManager::InitializeAudio);
		Instance->AudioPool.Initialize(World, UAudioComponent::StaticClass());
	}

	return Instance;
}

void USoundManager::InitializeAudio(UObject* AudioObject)
{
	UAudioComponent* Audio = Cast<UAudioComponent>(AudioObject);
	Audio->bAutoActivate = false;
}

void USoundManager::ResetAudio(UAudioComponent* Audio)
{
	if (Audio)
	{
		Audio->SetSound(nullptr);
		Audio->OnAudioFinishedNative.Clear();
		AudioPool.Put(Audio);
	}
}

void USoundManager::PlaySoundAtLocationByTag(const FName& SoundTag, const FVector& Location, float Volume, float Pitch)
{
	if (FSoundData* SoundData = SoundDataTable->FindRow<FSoundData>(SoundTag, FString()))
	{
		if (SoundData->Sound)
		{
			PlaySoundAtLocation(SoundData->Sound, Location, Volume, Pitch);
		}
	}
}

void USoundManager::PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float Volume, float Pitch)
{
	UAudioComponent* Audio = Cast<UAudioComponent>(AudioPool.Get());
	if (Audio && Sound)
	{
		Audio->SetSound(Sound);
		Audio->SetWorldLocation(Location);
		Audio->SetVolumeMultiplier(Volume);
		Audio->SetPitchMultiplier(Pitch);
		Audio->OnAudioFinishedNative.AddUObject(this, &USoundManager::ResetAudio);
		Audio->Play();
	}
}
