// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/Pool.h"

TPool::TPool()
{
}

TPool::~TPool()
{
}

void TPool::End()
{
	for (UObject* Object : ObjectPool)
	{
		Object->RemoveFromRoot();
	}
}

void TPool::Initialize(UWorld* InWorld, TSubclassOf<UObject> InObjectClass, AActor* InParentActor, int32 InPoolSize, int32 InPoolScaleStep)
{
	World = InWorld;
	ObjectClass = InObjectClass;
	ParentActor = InParentActor;
	PoolSize = 0;
	PoolScaleStep = InPoolScaleStep;
	CurrentIndex = -1;

	bIsInitialized = true;
	bIsActorClass = ObjectClass.Get()->IsChildOf(AActor::StaticClass());
	bIsComponentClass = ObjectClass.Get()->IsChildOf(UActorComponent::StaticClass());

	Expand(InPoolSize);
}

UObject* TPool::Get()
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool이 초기화되지 않았습니다. TPool::Initialize를 먼저 호출해주세요."));
		return nullptr;
	}

	int Left = PoolSize;

	while (Left--)
	{
		CurrentIndex = (CurrentIndex + 1) % PoolSize;

		UObject* Object = ObjectPool[CurrentIndex];
		if (ObjectAvailableMap[Object])
		{
			ObjectAvailableMap[Object] = false;
			Enable(Object);
			return Object;
		}
	}

	Expand();

	return Get();
}

bool TPool::Put(UObject* Object)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool이 초기화되지 않았습니다. TPool::Initialize를 먼저 호출해주세요."));
		return false;
	}

	if (!Object->IsA(ObjectClass) || !ObjectAvailableMap.Contains(Object))
	{
		return false;
	}

	Disable(Object);

	ObjectAvailableMap[Object] = true;
	return true;
}

void TPool::Expand(int Size)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool이 초기화되지 않았습니다. TPool::Initialize를 먼저 호출해주세요."));
		return;
	}

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

	if (bIsActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AActor* Actor = World->SpawnActor(ObjectClass, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
		if (ParentActor)
		{
			//Actor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
		}

		Object = Actor;
	}
	// 풀링 대상이 컴포넌트일 경우 World에 등록
	else if (bIsComponentClass)
	{
		Object = NewObject<UActorComponent>(World, ObjectClass);
		Object->AddToRoot();
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

	Disable(Object);

	return Object;
}

void TPool::Enable(UObject* Object)
{
	if (bIsActorClass)
	{
		AActor* Actor = Cast<AActor>(Object);
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorTickEnabled(true);
	}
}

void TPool::Disable(UObject* Object)
{
	if (bIsActorClass)
	{
		AActor* Actor = Cast<AActor>(Object);
		UPrimitiveComponent* Primitive = Actor->GetComponentByClass<UPrimitiveComponent>();

		Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Primitive->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorTickEnabled(false);
		
		if (ParentActor)
		{
			Actor->SetActorLocationAndRotation(ParentActor->GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}
