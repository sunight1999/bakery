// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"

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
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	AddMovementInput(FVector::ForwardVector, MovementValue.Y);
	AddMovementInput(FVector::RightVector, MovementValue.X);
}
