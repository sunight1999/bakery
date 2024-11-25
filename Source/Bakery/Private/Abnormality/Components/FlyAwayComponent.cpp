// Fill out your copyright notice in the Description page of Project Settings.


#include "Abnormality/Components/FlyAwayComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UFlyAwayComponent::UFlyAwayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyAwayComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	CurrentLocation = Owner->GetActorLocation();

	NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	GetWorld()->GetTimerManager().SetTimer(RandomMoveTimer, this, &UFlyAwayComponent::MoveToRandomLocation, MoveInterval, true, 0.f);
}

void UFlyAwayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;

	if (bIsMoving)
	{
		if (FMath::IsNearlyZero(FVector::DistSquared2D(CurrentLocation, TargetPath[CurrentPathPoint])))
		{
			if (++CurrentPathPoint == TargetPath.Num())
			{
				bIsMoving = false;
			}
		}
		else
		{
			CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, TargetPath[CurrentPathPoint], DeltaTime, MoveSpeed);
		}
	}

	float ZOffset = Amplitude * FMath::Sin(FlySpeed * ElapsedTime);
	FVector NewLocation = CurrentLocation;
	NewLocation.Z += ZOffset;

	Owner->SetActorLocation(NewLocation);
}

void UFlyAwayComponent::SetFlyOptions(float InMoveSpeed, float InFlySpeed, float InAmplitude, float InMoveInterval)
{
	MoveSpeed = InMoveSpeed;
	FlySpeed = InFlySpeed;
	Amplitude = InAmplitude;
	MoveInterval = InMoveInterval;
}

void UFlyAwayComponent::MoveToRandomLocation()
{
	FVector RandomLocation;
	FNavLocation NavLocation;

	if (NavigationSystem->GetRandomPointInNavigableRadius(CurrentLocation, 1000.f, NavLocation))
	{
		FVector& TargetLocation = NavLocation.Location;

		UNavigationPath* NavigationPath = NavigationSystem->FindPathToLocationSynchronously(GetWorld(), CurrentLocation, TargetLocation);
		if (!NavigationPath && !NavigationPath->IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("적절한 경로를 찾을 수 없습니다."));
			return;
		}

		TargetPath.Empty();
		CurrentPathPoint = 0;
		for (FVector& PathPoint : NavigationPath->PathPoints)
		{
			PathPoint.Z = CurrentLocation.Z;
			TargetPath.Emplace(PathPoint);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("적절한 목표 위치를 찾을 수 없습니다."));
		return;
	}

	bIsMoving = true;
}

