// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 이동시 밀리는 조작감을 없애기 위해 가속도 제거
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxAcceleration = 2048000.f;
	CharacterMovementComponent->BrakingDecelerationWalking = 2048000.f;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 1080.f, 0.f);

	World = GetWorld();
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

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	AddMovementInput(FVector::ForwardVector, MovementValue.Y);
	AddMovementInput(FVector::RightVector, MovementValue.X);
}

/*bool APlayerPawn::DetectObstacles(FVector Direction, FHitResult& HitResult)
{
	//FVector Start = GetActorLocation() + FVector::UpVector * ObstacleDetectHeight;
	//FVector End = Start + Direction * ObstacleDetectDistance;
	//ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;

	//DrawDebugLine(World, Start, End, FColor::Red, true);

	//return World->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel);
	return true;
}*/

