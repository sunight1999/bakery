// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

struct FInteractionInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterInteractDelegate, const FInteractionInfo&, InteractionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitInteractDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnEnterInteractDelegate OnEnterInteractDelegate;	// 상호작용 키를 눌렀을 때 콜백

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractDelegate OnInteractDelegate;	// 상호작용 키를 누르고 있을 때 콜백

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnExitInteractDelegate OnExitInteractDelegate;	// 상호작용 키를 뗐을 때 콜백
};
