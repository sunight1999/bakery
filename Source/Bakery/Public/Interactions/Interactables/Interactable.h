// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 상호작용 이벤트에 등록되는 기능 인터페이스
 * - 구현 시 UFUNCTION(BlueprintCallable) 매크로를 추가해야 함
 */
class BAKERY_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void OnEnterInteract() = 0;
	virtual void OnInteract() = 0;
	virtual void OnExitInteract() = 0;
};
