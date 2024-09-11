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
 * ��ȣ�ۿ� �̺�Ʈ�� ��ϵǴ� ��� �������̽�
 * - ���� �� UFUNCTION(BlueprintCallable) ��ũ�θ� �߰��ؾ� ��
 */
class BAKERY_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void OnEnterInteract() = 0;
	virtual void OnInteract() = 0;
	virtual void OnExitInteract() = 0;
};
