// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrabberComponent.generated.h"

class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UGrabberComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGrabberComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grab(FHitResult HitResult);
	void ReleaseBegin();
	void Release();

	bool IsGrabbing();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Interactor")
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, Category = "Interactor")
	USceneComponent* GrabPoint;
};