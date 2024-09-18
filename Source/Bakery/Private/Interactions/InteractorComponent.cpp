// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Interactions/Interactables/InteractableComponent.h"
#include "Interactions/InteractionDefines.h"

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Grabber = CreateDefaultSubobject<UGrabberComponent>(TEXT("Grabber"));
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	Grabber->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInteracting)
	{
		CurrentInteractable->OnInteractDelegate.Broadcast();
	}
	else if (bIsGrabbing)
	{
		CurrentInteractable->OnGrabDelegate.Broadcast();
	}
}

void UInteractorComponent::BeginInteraction()
{
	if (bIsGrabbing)
	{
		return;
	}

	FHitResult HitResult;
	if (DetectInteractable(InteractTraceChannel, HitResult))
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *HitResult.GetActor()->GetActorNameOrLabel());

		auto Interactable = HitResult.GetActor()->GetComponentByClass<UInteractableComponent>();
		if (!Interactable)
		{
			return;
		}

		CurrentInteractable = Interactable;
		CurrentInteractable->OnEnterInteractDelegate.Broadcast(FInteractionInfo(this, HitResult));
		bIsInteracting = true;
	}
}

void UInteractorComponent::EndInteraction()
{
	if (bIsGrabbing)
	{
		return;
	}

	if (CurrentInteractable)
	{
		CurrentInteractable->OnExitInteractDelegate.Broadcast();
		CurrentInteractable = nullptr;
	}

	bIsInteracting = false;
}

void UInteractorComponent::BeginGrab()
{
	if (bIsInteracting)
	{
		return;
	}

	FHitResult HitResult;
	if (DetectInteractable(GrabTraceChannel, HitResult))
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *HitResult.GetActor()->GetActorNameOrLabel());

		auto Interactable = HitResult.GetActor()->GetComponentByClass<UInteractableComponent>();
		if (!Interactable)
		{
			return;
		}

		CurrentInteractable = Interactable;
		CurrentInteractable->OnEnterGrabDelegate.Broadcast(FInteractionInfo(this, HitResult));
		bIsGrabbing = true;
	}
}

void UInteractorComponent::EndGrab()
{
	if (bIsInteracting)
	{
		return;
	}

	if (CurrentInteractable)
	{
		CurrentInteractable->OnExitGrabDelegate.Broadcast();
		CurrentInteractable = nullptr;
	}

	bIsGrabbing = false;
}

bool UInteractorComponent::DetectInteractable(const TEnumAsByte<ECollisionChannel>& TraceChannel, FHitResult& OutHitResult)
{
	FVector Start = GetComponentLocation() + FVector::UpVector * DetectHeight;
	FVector End = Start + GetForwardVector() * DetectDistance;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, TraceChannel);
}

