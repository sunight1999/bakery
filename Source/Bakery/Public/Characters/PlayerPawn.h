// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class BAKERY_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category="Movement")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Movement")
	UInputAction* MoveAction;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 3.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MeshTurnOffset = -90.f;

	const UWorld* World;
};
