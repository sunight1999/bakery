// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../Bakery/BakeryDefines.h"
#include "BaseCharacter.generated.h"

UCLASS()
class BAKERY_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaSeconds) override;

protected:
	/*
	 * Montage Functions
	 */
	void PlayMoveMontage(const FName& SectionName);

	UPROPERTY(EditAnywhere, Category = "Customer")
	UAnimMontage* MoveMontage;
};