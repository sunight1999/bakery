// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "DishThiefAbnormalityEvent.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API ADishThiefAbnormalityEvent : public AAbnormalityEvent
{
	GENERATED_BODY()
	
public:
	ADishThiefAbnormalityEvent();
	void Tick(float DeltaTime) override;

	void Cause() override;
	void Finish() override;

	bool bIsCaused = false;

private:
	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	float FlySpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	float Amplitude = 30.f;

	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	float MoveInterval = 5.f;
};
