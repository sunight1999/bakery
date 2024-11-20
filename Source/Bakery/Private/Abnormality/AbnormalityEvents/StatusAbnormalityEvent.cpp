// Fill out your copyright notice in the Description page of Project Settings.


#include "Abnormality/AbnormalityEvents/StatusAbnormalityEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerPawn.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "General/BakeryGameMode.h"

AStatusAbnormalityEvent::AStatusAbnormalityEvent()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStatusAbnormalityEvent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CharacterMovement = Player->GetCharacterMovement();
}

void AStatusAbnormalityEvent::Tick(float DeltaTime)
{
	if (bIsCaused)
	{
		Player->SetPlayerState(EPlayerState::Normal);

		if (WalkSpeedMultiplier != 1.f)
		{
			if (Player->IsGrabbing())
			{
				Player->SetPlayerState(EPlayerState::SpeedDown);
				CharacterMovement->MaxWalkSpeed = OriginWalkSpeed * WalkSpeedMultiplier;
			}
			else
			{
				CharacterMovement->MaxWalkSpeed = OriginWalkSpeed;
			}
		}
	}
}

void AStatusAbnormalityEvent::Cause()
{
	bIsCaused = true;

	Player->SetPlayerState(EPlayerState::Normal);

	if (WalkSpeedMultiplier != 1.f)
	{
		OriginWalkSpeed = CharacterMovement->MaxWalkSpeed;
	}
}

void AStatusAbnormalityEvent::Finish()
{
	bIsCaused = false;

	Player->SetPlayerState(EPlayerState::Normal);

	if (WalkSpeedMultiplier != 1.f)
	{
		CharacterMovement->MaxWalkSpeed = OriginWalkSpeed;
	}
}
