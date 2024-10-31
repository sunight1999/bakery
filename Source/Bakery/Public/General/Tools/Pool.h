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

	UObject* Get();
	void Put(UObject* PoolObject);

	FPostSpawnDelegate OnPostSpawned;

private:
	void Expand(int Size = -1);
	UObject* Spawn();

	bool bIsInitialized = false;

	TArray<UObject*> ObjectPool;
	TMap<UObject*, bool> ObjectAvailableMap;
	int CurrentIndex;

	UWorld* World;
	TSubclassOf<UObject> ObjectClass;
	AActor* ParentActor;
	int PoolSize = 0;
	int PoolScaleStep = 0;
};
