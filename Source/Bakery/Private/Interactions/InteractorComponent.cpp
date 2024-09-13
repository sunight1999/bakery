// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/InteractorComponent.h"
#include "Interactions/Interactables/InteractableComponent.h"

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsInteracting)
	{
		CurrentInteractable->OnInteractDelegate.Broadcast();
	}
}

void UInteractorComponent::BeginInteraction()
{
	FHitResult HitResult;
	
	if (DetectInteractable(HitResult))
	{
		auto Interactable = HitResult.GetActor()->GetComponentByClass<UInteractableComponent>();
		if (!Interactable)
		{
			return;
		}

		CurrentInteractable = Interactable;
		CurrentInteractable->OnEnterInteractDelegate.Broadcast();
		IsInteracting = true;
	}
}

void UInteractorComponent::EndInteraction()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->OnExitInteractDelegate.Broadcast();
		CurrentInteractable = nullptr;
	}

	IsInteracting = false;
}

bool UInteractorComponent::DetectInteractable(FHitResult& OutHitResult)
{
	FVector Start = GetComponentLocation() + FVector::UpVector * DetectHeight;
	FVector End = Start + GetForwardVector() * DetectDistance;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, TraceChannel);
}

