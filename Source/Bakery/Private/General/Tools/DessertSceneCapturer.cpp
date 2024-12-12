// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/DessertSceneCapturer.h"

ADessertSceneCapturer::ADessertSceneCapturer()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	DessertScene = CreateDefaultSubobject<USceneComponent>(TEXT("DessertScene"));
	DessertScene->SetupAttachment(RootComponent);

	BottomDessertMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomDessertMesh"));
	BottomDessertMesh->SetupAttachment(DessertScene);

	BodyDessertMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyDessertMesh"));
	BodyDessertMesh->SetupAttachment(DessertScene);

	LidDessertMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidDessertMesh"));
	LidDessertMesh->SetupAttachment(DessertScene);
}

void ADessertSceneCapturer::BeginPlay()
{
	Super::BeginPlay();

}

void ADessertSceneCapturer::SetDessertMesh(UStaticMesh* BottomMesh, UStaticMesh* BodyMesh, UStaticMesh* LidMesh)
{
	TArray<UStaticMesh*> Meshes{ BottomMesh, BodyMesh, LidMesh };
	for (int i = 0; i < 3; i++)
	{
		UStaticMeshComponent* MeshComponent = nullptr;
		UStaticMesh* Mesh = Meshes[i];

		switch (i)
		{
		case 0:
			MeshComponent = BottomDessertMesh;
			break;

		case 1:
			MeshComponent = BodyDessertMesh;
			break;

		case 2:
			MeshComponent = LidDessertMesh;
			break;
		}

		if (!Mesh)
		{
			MeshComponent->SetStaticMesh(nullptr);
			continue;
		}

		float* ScalePtr = ScalingMap.Find(Mesh);
		if (ScalePtr)
		{
			MeshComponent->SetRelativeScale3D(FVector(*ScalePtr));
		}
		else
		{
			MeshComponent->SetRelativeScale3D(FVector::OneVector);
		}

		MeshComponent->SetStaticMesh(Mesh);
	}
}
