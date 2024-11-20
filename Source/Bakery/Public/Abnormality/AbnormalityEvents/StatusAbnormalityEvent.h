// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "StatusAbnormalityEvent.generated.h"

class APlayerPawn;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class BAKERY_API AStatusAbnormalityEvent : public AAbnormalityEvent
{
	GENERATED_BODY()

public:
	AStatusAbnormalityEvent();
	void Tick(float DeltaTime) override;

	virtual void Cause() override;
	virtual void Finish() override;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="AbnormalityEvent|Status")
	float WalkSpeedMultiplier = 1.f;
	float OriginWalkSpeed;

	APlayerPawn* Player;
	UCharacterMovementComponent* CharacterMovement;

	bool bIsCaused = false;
};
