// Fill out your copyright notice in the Description page of Project Settings.


#include "Bakery/BakeryManager.h"

ABakeryManager::ABakeryManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABakeryManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABakeryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

