// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Interactions/Interactables/InteractableComponent.h"

// 제거 예정

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Interactable = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	Interactable->OnEnterInteractDelegate.AddDynamic(this, &ATestActor::OnEnterInteract);
	Interactable->OnInteractDelegate.AddDynamic(this, &ATestActor::OnInteract);
	Interactable->OnExitInteractDelegate.AddDynamic(this, &ATestActor::OnExitInteract);
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestActor::OnEnterInteract()
{
	UE_LOG(LogTemp, Display, TEXT("Enter"));
}

void ATestActor::OnInteract()
{
	UE_LOG(LogTemp, Display, TEXT("KEEP"));
}

void ATestActor::OnExitInteract()
{
	UE_LOG(LogTemp, Display, TEXT("EXIT"));
}

