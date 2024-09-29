// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/Cloner.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
ACloner::ACloner()
{
	PrimaryActorTick.bCanEverTick = false;

	//Box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//RootComponent = Box;

	ISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	RootComponent = ISM;

	Initialize();
}

/*void ACloner::Expand(EDirection Direction)
{

}*/

void ACloner::BeginPlay()
{
	Super::BeginPlay();
}

void ACloner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Initialize();
}

void ACloner::Initialize()
{
	ISM->ClearInstances();
	ISM->SetStaticMesh(StaticMesh);
	ISM->SetCanEverAffectNavigation(true);

	FVector OriginLocation = FVector(StartOffsetX, StartOffsetY, StartOffsetZ);
	FTransform Transform(FQuat::Identity, OriginLocation, FVector::OneVector);

	for (uint32 z = 0; z < ZNum; z++)
	{
		FVector Location = OriginLocation;

		if (z != 0)
			Location.Z += DistanceZ;

		for (uint32 x = 0; x < ForwardNum; x++)
		{
			if (x != 0)
				Location.X += DistanceX;

			double OriginY = Location.Y;
			for (uint32 y = 0; y < LeftNum; y++)
			{
				Location.Y -= DistanceY;
				Transform.SetLocation(Location);

				ISM->AddInstance(Transform);
				UE_LOG(LogTemp, Display, TEXT("%s"), *Location.ToCompactString());
			}

			Location.Y = OriginY;
			for (uint32 y = 0; y < RightNum; y++)
			{
				if (y != 0)
					Location.Y += DistanceY;
				Transform.SetLocation(Location);

				ISM->AddInstance(Transform);
			}

			Location.Y = OriginY;
		}
	}
}

