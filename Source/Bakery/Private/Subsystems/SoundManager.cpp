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

void USoundManager::PlayBackgroundMusic()
{
	BackgroundMusicAudio = PlaySoundAtLocationByTag(BACKGROUND_MUSIC_SOUND_TAG, FVector::ZeroVector);
	if (!BackgroundMusicAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("배경 음악 소스(%s)를 찾을 수 없습니다."), BACKGROUND_MUSIC_SOUND_TAG);
		return;
	}

	BackgroundMusicAudio->OnAudioFinishedNative.Clear();
	BackgroundMusicAudio->OnAudioFinishedNative.AddUObject(this, &USoundManager::OnBackgroundMusicEnded);
}

void USoundManager::StopBackgroundMusic()
{
	if (!BackgroundMusicAudio)
	{
		return;
	}

	BackgroundMusicAudio->Stop();
	BackgroundMusicAudio = nullptr;
}

void USoundManager::SetBackgroundMusicPitch(float PitchMultiplier)
{
	if (!BackgroundMusicAudio)
	{
		return;
	}

	BackgroundMusicAudio->PitchMultiplier = PitchMultiplier;
}

void USoundManager::OnBackgroundMusicEnded(UAudioComponent* Audio)
{
	ResetAudio(Audio);
	PlayBackgroundMusic();
}

/// <summary>
/// 'Content/Data/DT_Sound'에 정의된 효과음을 재생한다.
/// </summary>
/// <param name="SoundTag">DT_Sound 데이터 테이블 RowName</param>
/// <param name="Location">효과음 재생 위치</param>
/// <param name="Volume">Volume Multiplier</param>
/// <param name="Pitch">Pitch Multiplier</param>
/// <returns>효과음이 Loop일 경우 해당 효과음을 재생하고 있는 UAudioComponent* 반환. 아니면 nullptr 반환.
/// 반환된 UAudioComponent*는 Stop하면 자동으로 SoundManager의 풀로 다시 반환된다.</returns>
UAudioComponent* USoundManager::PlaySoundAtLocationByTag(const FName& SoundTag, const FVector& Location, float Volume, float Pitch)
{
	if (FSoundData* SoundData = SoundDataTable->FindRow<FSoundData>(SoundTag, FString()))
	{
		if (SoundData->Sound)
		{
			return PlaySoundAtLocation(SoundData->Sound, Location, Volume, Pitch, SoundData->bIsLoop);
		}
	}

	return nullptr;
}

UAudioComponent* USoundManager::PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float Volume, float Pitch, bool bIsLoop)
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

		if (bIsLoop)
		{
			return Audio;
		}
	}

	return nullptr;
}
