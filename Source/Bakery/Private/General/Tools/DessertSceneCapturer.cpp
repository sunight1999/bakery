// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/DessertSceneCapturer.h"

ADessertSceneCapturer::ADessertSceneCapturer()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	DessertScene = CreateDefaultSubobject<USceneComponent>(TEXT("DessertScene"));
	DessertScene->SetupAttachment(RootComponent);

	DessertMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DessertMesh"));
	DessertMesh->SetupAttachment(DessertScene);
}

void ADessertSceneCapturer::BeginPlay()
{
	Super::BeginPlay();

}

void ADessertSceneCapturer::SetDessertMesh(UStaticMesh* Mesh)
{
	if (Mesh)
	{
		float* ScalePtr = ScalingMap.Find(Mesh);
		if (ScalePtr)
		{
			DessertMesh->SetRelativeScale3D(FVector(*ScalePtr));
			DessertMesh->SetStaticMesh(Mesh);
		}
		else
		{
			DessertMesh->SetRelativeScale3D(FVector(1.f));
		}

		DessertMesh->SetStaticMesh(Mesh);
	}
}
