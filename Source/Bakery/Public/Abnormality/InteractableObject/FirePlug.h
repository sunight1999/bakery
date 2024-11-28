// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "FirePlug.generated.h"

/**
 * 소화전
 */
class UProgressWidget;
class UWidgetComponent;
UCLASS()
class BAKERY_API AFirePlug : public AInteractableActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	void OnInteract(float deltatime) override;
	void OnExitInteract()override;
public:
	UPROPERTY(EditAnywhere, Category = "FirePlug")
	float MaxGauge;
	UPROPERTY(EditAnywhere, Category = "FirePlug")
	float CurrentGauge = 0;
	UPROPERTY(EditAnywhere, Category = "FirePlug")
	float GuageSpeed = 0.1f;
	UPROPERTY(VisibleAnywhere, Category = "FirePlug")
	bool EventFlag;
	UWidgetComponent* ProgressComponent;
	UProgressWidget* ProgressWidget;

public:
	void SetEventFlag(bool Flag);
};
