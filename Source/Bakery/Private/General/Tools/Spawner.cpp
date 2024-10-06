// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/Spawner.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	World = GetWorld();

	TimerManager = &GetWorldTimerManager();
}

void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::Start()
{
	if (SpawnTimer.IsValid())
	{
		TimerManager->UnPauseTimer(SpawnTimer);
	}
	else
	{
		TimerManager->SetTimer(SpawnTimer, this, &ASpawner::Spawn, SpawnInterval, true, 0.f);
	}
}

void ASpawner::Stop()
{
	if (SpawnTimer.IsValid())
	{
		TimerManager->PauseTimer(SpawnTimer);
	}
}

void ASpawner::Spawn()
{
	// TODO: Spawn 수 추적해 최대 수 제한 필요

	if (SpawnChance < 1.f)
	{
		// 지정한 확률로 Spawn 실패
		float Chance = FMath::RandRange(0.f, 1.f);
		if (Chance > SpawnChance)
		{
			return;
		}
	}

	// 스폰 직후 충돌이 일어나면 적절히 위치를 조정하도록 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Actor = World->SpawnActor(SpawnActorClass, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	Actor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PostSpawn(Actor);
}

void ASpawner::Despawn(AActor* Actor)
{
}
