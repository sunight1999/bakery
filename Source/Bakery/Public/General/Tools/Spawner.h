// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawn.h"
#include "Spawner.generated.h"

class ISpawn;

UCLASS()
class BAKERY_API ASpawner : public AActor, public ISpawn
{
	GENERATED_BODY()
	
public:	
	ASpawner();
	virtual void Tick(float DeltaTime) override;

	void Start();
	void Stop();

	void Spawn();

	virtual void PostSpawn(AActor* Actor) override {}

protected:
	virtual void BeginPlay() override;

	UWorld* World;

private:
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(EditAnywhere, Category="Spawner")
	float SpawnInterval = 10.f;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (UIMin = "0.0", UIMax = "1.0", ClapMin = "0.0", ClampMax="1.0"))
	float SpawnChance = 1.f;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	int MaxSpawnNum = 15;
	
	FTimerManager* TimerManager;
	FTimerHandle SpawnTimer;
};
