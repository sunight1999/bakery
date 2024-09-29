// Fill out your copyright notice in the Description page of Project Settings.


#include "Hall/Chair.h"
#include "Hall/Table.h"

AChair::AChair()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMesh;
}

ATable* AChair::GetOwnerTable()
{
	if (!OwnerTable)
	{
		OwnerTable = Cast<ATable>(GetOwner());
	}

	return OwnerTable;
}

void AChair::BeginPlay()
{
	Super::BeginPlay();
	
}
