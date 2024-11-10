// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE_OneParam(FPostSpawnDelegate, UObject*);

/**
 * 
 */
class BAKERY_API TPool
{
public:
	TPool();
	~TPool();

	void Initialize(UWorld* InWorld, TSubclassOf<UObject> InObjectClass, AActor* InParentActor = nullptr, int32 InPoolSize = 10, int32 InPoolScaleStep = 10);
	void Expand(int Size = -1);

	UObject* Get();
	bool Put(UObject* PoolObject);

	// 오브젝트 생성 후 추가로 수행할 작업 델리게이트. 반드시 Initialize 호출 전에 바인딩해야 함.
	FPostSpawnDelegate OnPostSpawned;

private:
	UObject* Spawn();
	void Enable(UObject* Object);
	void Disable(UObject* Object);

	bool bIsInitialized = false;
	bool bIsActorClass = false;
	bool bIsComponentClass = false;

	TArray<UObject*> ObjectPool;
	TMap<UObject*, bool> ObjectAvailableMap;
	int CurrentIndex;

	UWorld* World;
	TSubclassOf<UObject> ObjectClass;
	AActor* ParentActor;
	int PoolSize = 0;
	int PoolScaleStep = 0;
};
