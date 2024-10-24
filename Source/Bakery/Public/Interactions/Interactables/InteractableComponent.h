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

	// TODO: Interactable Component 내부에서 (IInteract를 구현하는)Owner Actor의 함수 알아서 바인딩하기

	/*
	 * 상호작용 델리게이트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionInfoDelegate OnEnterInteractDelegate;	// 상호작용 키를 눌렀을 때 콜백

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FInteractionDelegate OnInteractDelegate;	// 상호작용 키를 누르고 있을 때 콜백

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FInteractionDelegate OnExitInteractDelegate;	// 상호작용 키를 뗐을 때 콜백

	/*
	 * 잡기 델리게이트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionInfoDelegate OnEnterGrabDelegate;	// 상호작용 키를 눌렀을 때 콜백

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnGrabDelegate;	// 상호작용 키를 누르고 있을 때 콜백

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnExitGrabDelegate;	// 상호작용 키를 뗐을 때 콜백
};
