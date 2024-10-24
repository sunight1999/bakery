// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class UInteractorComponent;

UCLASS()
class BAKERY_API APlayerPawn : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void StartBakery();

private:
	/*
	 * Input Actions
	 */
	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* StartBakeryAction;

	/*
	 * 상호작용 및 이동 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Player|Interaction")
	UInteractorComponent* Interactor;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	float TurnSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	float MeshTurnOffset = -90.f;
};
