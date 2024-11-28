// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Components/FloatingComponent.h"

UFloatingComponent::UFloatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFloatingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFloatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;

	if (bIsBeating)
	{
		float Offset = Beating * FMath::Sin(BeatingSpeed * ElapsedTime);
		FVector Scale = OriginScale;
		Scale += FVector::OneVector * Offset;

		GetOwner()->SetActorScale3D(Scale);
	}

	if (bIsFloating)
	{
		float Offset = Amplitude * FMath::Sin(FloatingSpeed * ElapsedTime);
		FVector Location = OriginLocation;
		Location.Z += Offset;

		GetOwner()->SetActorLocation(Location);
	}
}

void UFloatingComponent::Reset(const FVector& Location, const FVector& Scale)
{
	ElapsedTime = 0.f;

	OriginLocation = Location;
	OriginScale = Scale;

	GetOwner()->SetActorLocation(OriginLocation);
	GetOwner()->SetActorScale3D(OriginScale);
}
