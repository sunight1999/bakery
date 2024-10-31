// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "General/Tools/Pool.h"
#include "SoundManager.generated.h"

class UAudioComponent;

USTRUCT(BlueprintType)
struct FSoundData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SoundTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Sound;
};

/**
 * 
 */
UCLASS()
class BAKERY_API USoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	USoundManager();
	~USoundManager();
	static USoundManager* GetInstance(UWorld* World);

	void InitializeAudio(UObject* AudioObject);
	void ResetAudio(UAudioComponent* Audio);

	void PlaySoundAtLocationByTag(const FName& SoundTag, const FVector& Location, float Volume = 1.f, float Pitch = 1.f);
	void PlaySoundAtLocation(USoundBase* Sound, const FVector& Location, float Volume = 1.f, float Pitch = 1.f);

private:
	static USoundManager* Instance;

	UDataTable* SoundDataTable = nullptr;
	TPool AudioPool;
};
