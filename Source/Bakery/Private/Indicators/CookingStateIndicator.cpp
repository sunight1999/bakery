// Fill out your copyright notice in the Description page of Project Settings.


#include "Indicators/CookingStateIndicator.h"
#include "General/Components/FloatingComponent.h"

ACookingStateIndicator::ACookingStateIndicator()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootSceneComponent->SetWorldScale3D(FVector::OneVector);
	RootComponent = RootSceneComponent;

	CookingDoneIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CookingDoneIconMesh"));
	CookingDoneIconMesh->SetCastShadow(false);
	CookingDoneIconMesh->SetupAttachment(RootComponent);

	CookingFailedIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CookingFailedIconMesh"));
	CookingFailedIconMesh->SetCastShadow(false);
	CookingFailedIconMesh->SetupAttachment(RootComponent);

	CookingNextTargetIconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CookingNextTargetIconMesh"));
	CookingNextTargetIconMesh->SetCastShadow(false);
	CookingNextTargetIconMesh->SetupAttachment(RootComponent);

	FloatingComponent = CreateDefaultSubobject<UFloatingComponent>(TEXT("FloatingComponent"));
}

void ACookingStateIndicator::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingStateIndicator::Initailize(const FVector& Location, const FVector& Scale)
{
	OriginLocation = Location;
	OriginScale = Scale;

	Hide();
}

void ACookingStateIndicator::Show(ECookingIndicator CookingIndicator)
{
	SetActorHiddenInGame(false);
	FloatingComponent->SetComponentTickEnabled(true);

	switch (CookingIndicator)
	{
	case ECookingIndicator::Done:
		CookingDoneIconMesh->SetHiddenInGame(false);
		break;

	case ECookingIndicator::Failed:
		CookingFailedIconMesh->SetHiddenInGame(false);
		break;

	case ECookingIndicator::Target:
		CookingNextTargetIconMesh->SetHiddenInGame(false);
		break;
	}
}

void ACookingStateIndicator::Hide()
{
	SetActorHiddenInGame(true);
	FloatingComponent->SetComponentTickEnabled(false);

	CookingDoneIconMesh->SetHiddenInGame(true);
	CookingFailedIconMesh->SetHiddenInGame(true);
	CookingNextTargetIconMesh->SetHiddenInGame(true);

	FloatingComponent->Reset(OriginLocation, OriginScale);
}

