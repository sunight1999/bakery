// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UFloatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFloatingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Reset(const FVector& Location, const FVector& Scale);

protected:
	virtual void BeginPlay() override;

private:
	// true일 경우, 액터가 BeatingSpeed 속도로 커졌다 작아짐
	UPROPERTY(EditAnywhere)
	bool bIsBeating = true;

	// true일 경우, 액터가 FloatingSpeed 속도로 위아래로 움직임
	UPROPERTY(EditAnywhere)
	bool bIsFloating = true;
	
	UPROPERTY(EditAnywhere)
	float BeatingSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float FloatingSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	float Beating = 0.03f;

	UPROPERTY(EditAnywhere)
	float Amplitude = 15.f;

	float ElapsedTime = 0.f;

	FVector OriginLocation;
	FVector OriginScale;
};
