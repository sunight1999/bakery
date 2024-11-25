// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

struct FInteractionInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionInfoDelegate, const FInteractionInfo&, InteractionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	/*
	 * 상호작용 델리게이트
	 */
	 // 상호작용 키를 눌렀을 때 콜백
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionInfoDelegate OnEnterInteractDelegate;

	// 상호작용 키를 누르고 있을 때 콜백
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FInteractionDelegate OnInteractDelegate;

	// 상호작용 키를 뗐을 때 콜백
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FInteractionDelegate OnExitInteractDelegate;

	/*
	 * 잡기 델리게이트
	 */
	 // 상호작용 키를 눌렀을 때 콜백
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionInfoDelegate OnEnterGrabDelegate;

	// 상호작용 키를 누르고 있을 때 콜백
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnGrabDelegate;

	// 상호작용 키를 뗐을 때 콜백
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnExitGrabDelegate;
};
