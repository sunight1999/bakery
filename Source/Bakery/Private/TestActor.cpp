// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Interactions/InteractionDefines.h"

// 제거 예정

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestActor::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	UE_LOG(LogTemp, Display, TEXT("ENTER"));
}

void ATestActor::OnInteract()
{
	UE_LOG(LogTemp, Display, TEXT("KEEP"));
}

void ATestActor::OnExitInteract()
{
	UE_LOG(LogTemp, Display, TEXT("EXIT"));
}

