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
		UE_LOG(LogTemp, Display, TEXT("%s"), *HitResult.GetActor()->GetActorNameOrLabel());

		auto Interactable = HitResult.GetActor()->GetComponentByClass<UInteractableComponent>();
		if (!Interactable)
		{
			return;
		}

		// Grab Interactable인 경우 PhysicsHandle을 이용해 잡기
		if (Interactable->ComponentHasTag(INTERACTABLE_COMPONENT_GRAB))
		{
			Grabber->Grab(HitResult);
		}

		CurrentInteractable = Interactable;
		CurrentInteractable->OnEnterInteractDelegate.Broadcast(FInteractionInfo(this, HitResult));
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

