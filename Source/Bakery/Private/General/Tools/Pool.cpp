// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/Pool.h"

TPool::TPool()
{
}

TPool::~TPool()
{
}

void TPool::Initialize(UWorld* InWorld, TSubclassOf<UObject> InObjectClass, AActor* InParentActor, int32 InPoolSize, int32 InPoolScaleStep)
{
	World = InWorld;
	ObjectClass = InObjectClass;
	ParentActor = InParentActor;
	PoolSize = 0;
	PoolScaleStep = InPoolScaleStep;
	CurrentIndex = -1;

	Expand(InPoolSize);

	bIsInitialized = true;
}

UObject* TPool::Get()
{
	int Left = PoolSize;

	while (Left--)
	{
		CurrentIndex = (CurrentIndex + 1) % PoolSize;

		UObject* Object = ObjectPool[CurrentIndex];
		if (ObjectAvailableMap[Object])
		{
			ObjectAvailableMap[Object] = false;
			return Object;
		}
	}

	Expand();

	return Get();
}

void TPool::Put(UObject* Object)
{
	if (!Object->IsA(ObjectClass) || !ObjectAvailableMap.Contains(Object))
	{
		UE_LOG(LogTemp, Warning, TEXT("유효하지 않은 오브젝트입니다."));
		return;
	}

	ObjectAvailableMap[Object] = true;
}

void TPool::Expand(int Size)
{
	if (Size == -1)
	{
		Size = PoolScaleStep;
	}

	for (int i = 0; i < Size; i++)
	{
		UObject* Object = Spawn();
		ObjectPool.Emplace(Object);
		ObjectAvailableMap.Emplace(Object, true);
	}

	PoolSize += Size;
}

UObject* TPool::Spawn()
{
	UObject* Object = nullptr;

	if (ObjectClass.Get()->IsChildOf(AActor::StaticClass()))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AActor* Actor = World->SpawnActor(ObjectClass, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);

		if (ParentActor)
		{
			Actor->AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}

		Object = Actor;
	}
	// 풀링 대상이 컴포넌트일 경우 World에 등록
	else if (ObjectClass.Get()->IsChildOf(UActorComponent::StaticClass()))
	{
		Object = NewObject<UActorComponent>(World, ObjectClass);
		UActorComponent* Component = Cast<UActorComponent>(Object);
		Component->RegisterComponentWithWorld(World);
	}
	else
	{
		Object = NewObject<UObject>(World, ObjectClass);
	}
	
	if (OnPostSpawned.IsBound())
	{
		OnPostSpawned.Execute(Object);
	}

	return Object;
}
