// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/InteractorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Interactions/GrabberComponent.h"
#include "Interactions/Interactables/Interact.h"
#include "Interactions/Interactables/Grab.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/Interactables/Highlight.h"
#include "Characters/PlayerPawn.h"
#include "Subsystems/UISubsystem.h"

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

	// 상호작용 가능한 액터 하이라이팅 처리
	FHitResult HitResult;
	if (DetectInteractable(InteractTraceChannel, HitResult))
	{
		AActor* DetectedActor = HitResult.GetActor();
		if (PrevDetectedActor != DetectedActor)
		{
			if (PrevDetectedActor)
			{
				if (auto Highlightable = Cast<IHighlight>(PrevDetectedActor))
				{
					Highlightable->OnExitHighlight();
				}
			}

			if (auto Highlightable = Cast<IHighlight>(DetectedActor))
			{
				Highlightable->OnEnterHighlight();
			}

			PrevDetectedActor = DetectedActor;
		}
	}
	else
	{
		if (auto Highlightable = Cast<IHighlight>(PrevDetectedActor))
		{
			Highlightable->OnExitHighlight();
		}

		PrevDetectedActor = nullptr;
	}

	// 상호작용 중인 액터 처리
	if (CurrentInteracting)
	{
		CurrentInteracting->OnInteract(DeltaTime);
	}
	else if (CurrentGrabbing)
	{
		CurrentGrabbing->OnGrab(DeltaTime);
	}
}

void UInteractorComponent::BeginInteraction()
{
	// UI가 열려 있을 경우 처리
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn && PlayerPawn->IsUIOpened())
	{
		UUISubsystem* UISubsystem = PlayerPawn->GetGameInstance()->GetSubsystem<UUISubsystem>();
		UISubsystem->RevertUILayer();
		PlayerPawn->SetUIOpened(false);

		return;
	}

	if (CurrentGrabbing)
	{
		return;
	}

	FHitResult HitResult;
	if (DetectInteractable(InteractTraceChannel, HitResult))
	{
		CurrentInteracting = Cast<IInteract>(HitResult.GetActor());
		if (!CurrentInteracting)
		{
			return;
		}

		CurrentInteracting->OnEnterInteract(FInteractionInfo(this, HitResult));
	}
}

void UInteractorComponent::EndInteraction()
{
	if (CurrentGrabbing)
	{
		return;
	}

	if (CurrentInteracting)
	{
		CurrentInteracting->OnExitInteract();
		CurrentInteracting = nullptr;
	}
}

void UInteractorComponent::BeginGrab()
{
	if (CurrentInteracting)
	{
		return;
	}

	FHitResult HitResult;
	if (DetectInteractable(GrabTraceChannel, HitResult))
	{
		CurrentGrabbing = Cast<IGrab>(HitResult.GetActor());
		if (!CurrentGrabbing)
		{
			return;
		}

		CurrentGrabbing->OnEnterGrab(FInteractionInfo(this, HitResult));
	}
}

void UInteractorComponent::EndGrab()
{
	if (CurrentInteracting)
	{
		return;
	}

	if (CurrentGrabbing)
	{
		CurrentGrabbing->OnExitGrab();
		CurrentGrabbing = nullptr;
	}
}

bool UInteractorComponent::DetectInteractable(const TEnumAsByte<ECollisionChannel>& TraceChannel, FHitResult& OutHitResult)
{
	FVector Start = GetComponentLocation() + FVector::UpVector * DetectHeight;
	FVector End = Start + GetForwardVector() * DetectDistance;

	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, TraceChannel);
}

