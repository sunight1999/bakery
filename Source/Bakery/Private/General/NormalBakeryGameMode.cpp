// Fill out your copyright notice in the Description page of Project Settings.


#include "General/NormalBakeryGameMode.h"

void ANormalBakeryGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalBakeryGameMode::Disappointed(APawn* Pawn)
{
	UE_LOG(LogTemp, Display, TEXT("우우우"));

	GetWorld()->GetFirstPlayerController()->GameHasEnded(Pawn, false);
}
