// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 이동시 밀리는 조작감을 없애기 위해 가속도 제거
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxAcceleration = 2048000.f;
	CharacterMovementComponent->BrakingDecelerationWalking = 2048000.f;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 1080.f, 0.f);
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
}


void ABaseCharacter::PlayMoveMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && MoveMontage)
	{
		AnimInstance->Montage_Play(MoveMontage);
		AnimInstance->Montage_JumpToSection(SectionName, MoveMontage);
	}
}
