// Fill out your copyright notice in the Description page of Project Settings.


#include "Abnormality/Components/FlyAwayComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UFlyAwayComponent::UFlyAwayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyAwayComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	CurrentLocation = Owner->GetActorLocation();

	APawn* Pawn = Cast<APawn>(Owner);
	DishController = CastChecked<AAIController>(Pawn->GetController());
	NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	GetWorld()->GetTimerManager().SetTimer(RandomMoveTimer, this, &UFlyAwayComponent::MoveToRandomLocation, MoveInterval, true);
}

void UFlyAwayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;

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
	FVector Origin = Owner->GetActorLocation();
	FVector RandomLocation;
	FNavLocation NavLocation;

	if (NavigationSystem->GetRandomPointInNavigableRadius(Origin, 1000.f, NavLocation))
	{
		RandomLocation = NavLocation.Location;
		DishController->MoveToLocation(RandomLocation, -1.f, false);
		UE_LOG(LogTemp, Warning, TEXT("적저라한.")); // 호출은 되는데 이동이 안 됨
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("적절한 이동 경로를 찾을 수 없습니다."));
	}
}

