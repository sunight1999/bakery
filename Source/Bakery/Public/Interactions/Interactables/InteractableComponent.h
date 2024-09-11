// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitInteractDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interactions")
	FOnEnterInteractDelegate OnEnterInteractDelegate;

	UPROPERTY(BlueprintAssignable, Category="Interactions")
	FOnInteractDelegate OnInteractDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Interactions")
	FOnExitInteractDelegate OnExitInteractDelegate;
};
