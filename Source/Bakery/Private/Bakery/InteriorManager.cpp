// Fill out your copyright notice in the Description page of Project Settings.


#include "Bakery/InteriorManager.h"
#include "Bakery/BakeryDefines.h"
#include "General/Tools/Cloner.h"

AInteriorManager::AInteriorManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteriorManager::OnConstruction(const FTransform& Transform)
{
	if (!BakeryFloors)
	{
		return;
	}

	for (AActor* Wall : BakeryWalls)
	{
		if (!Wall)
			return;
	}

	for (AActor* Corner : BakeryCornerWalls)
	{
		if (!Corner)
			return;
	}

	const float FloorTileLengthY = BakeryFloors->GetDistanceY();
	const float FloorTileLengthX = BakeryFloors->GetDistanceX();

	float LeftLength = FloorTileLengthY * BakeryFloors->GetLeftNum();
	float RightLength = FloorTileLengthY * BakeryFloors->GetRightNum();
	float ForwardLength = FloorTileLengthX * BakeryFloors->GetForwardNum();

	FVector Left = FVector::LeftVector * LeftLength;
	FVector Right = FVector::RightVector * RightLength;
	FVector Forward = FVector::ForwardVector * ForwardLength;

	BakeryWalls[(uint8)EDirection::Left]->SetActorRelativeLocation(Left);
	BakeryWalls[(uint8)EDirection::Right]->SetActorRelativeLocation(Right);
	BakeryWalls[(uint8)EDirection::Forward]->SetActorRelativeLocation(Forward);

	BakeryCornerWalls[(uint8)EDirection::Left]->SetActorRelativeLocation(Left + Forward);
	BakeryCornerWalls[(uint8)EDirection::Right]->SetActorRelativeLocation(Right + Forward);

	/*
	for (int i = 0; i < 4; i++)
	{

	}
	*/
}

void AInteriorManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteriorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

