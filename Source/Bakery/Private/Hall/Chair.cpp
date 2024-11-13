// Fill out your copyright notice in the Description page of Project Settings.


#include "Hall/Chair.h"
#include "Hall/Table.h"

AChair::AChair()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMesh;

	SitPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SitPoint"));
	SitPoint->SetupAttachment(RootComponent);
}

void AChair::Tick(float DeltaTime)
{
	if (bIsMoving)
	{
		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MovingTargetLocation, DeltaTime, PullOutSpeed);
		SetActorLocation(NewLocation);

		if (FMath::IsNearlyZero(FVector::DistSquared2D(NewLocation, MovingTargetLocation)))
		{
			bIsMoving = false;
		}
	}
}

void AChair::Initialize()
{
	// 손님이 자리를 할당 받았지만 앉기 전에 영업이 끝나는 경우, 의자가 들어가 있는 상태에서 PutInChair()가 호출되므로 Location 정보를 별도로 계산
	PullOutLocation = GetActorLocation() + GetActorForwardVector() * -PullOutDistance;
}

void AChair::PullOutChair()
{
	MovingTargetLocation = PullOutLocation;
	bIsMoving = true;
}

void AChair::PutInChair()
{
	MovingTargetLocation = PullOutLocation + GetActorForwardVector() * PullOutDistance;
	bIsMoving = true;
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
