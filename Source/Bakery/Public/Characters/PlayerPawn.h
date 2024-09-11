// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UInteractorComponent;

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

	UPROPERTY(EditAnywhere, Category="Player Movement")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Interactor")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Interactor")
	UInteractorComponent* Interactor;

private:
	UPROPERTY(EditAnywhere, Category = "Player Movement")
	float TurnSpeed = 3.f;

	UPROPERTY(EditAnywhere, Category = "Player Movement")
	float MeshTurnOffset = -90.f;
};
