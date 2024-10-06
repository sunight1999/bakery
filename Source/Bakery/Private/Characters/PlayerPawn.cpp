// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "General/BakeryGameMode.h"
#include "General/BakeryGameState.h"

APlayerPawn::APlayerPawn()
{
	Interactor = CreateDefaultSubobject<UInteractorComponent>(TEXT("Interactor"));
	Interactor->SetupAttachment(RootComponent);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	verify(MoveAction);
	verify(InteractAction);
	verify(GrabAction);
	
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, Interactor, &UInteractorComponent::BeginInteraction);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Completed, Interactor, &UInteractorComponent::EndInteraction);
		EnhancedInput->BindAction(GrabAction, ETriggerEvent::Started, Interactor, &UInteractorComponent::BeginGrab);
		EnhancedInput->BindAction(GrabAction, ETriggerEvent::Completed, Interactor, &UInteractorComponent::EndGrab);
		EnhancedInput->BindAction(StartBakeryAction, ETriggerEvent::Started, this, &APlayerPawn::StartBakery);
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	AddMovementInput(FVector::ForwardVector, MovementValue.Y);
	AddMovementInput(FVector::RightVector, MovementValue.X);
}

void APlayerPawn::StartBakery()
{
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	if (BakeryGameState->GetBakeryState() == EBakeryState::Opened)
	{
		return;
	}

	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	BakeryGameMode->OpenBakery();
}
