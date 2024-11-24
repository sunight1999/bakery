// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlyAwayComponent.generated.h"

class AAIController;
class UNavigationSystemV1;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UFlyAwayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFlyAwayComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetFlyOptions(float InMoveSpeed, float InFlySpeed, float InAmplitude, float InMoveInterval);

	void MoveToRandomLocation();

protected:
	virtual void BeginPlay() override;

private:
	float MoveSpeed = 100.f;
	float FlySpeed = 100.f;
	float Amplitude = 30.f;
	float MoveInterval = 5.f;

	AActor* Owner = nullptr;
	AAIController* DishController = nullptr;
	UNavigationSystemV1* NavigationSystem = nullptr;

	FVector CurrentLocation;
	float ElapsedTime = 0.f;

	FTimerHandle RandomMoveTimer;
};
