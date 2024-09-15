// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractorComponent.generated.h"

class UGrabberComponent;
class UInteractableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UInteractorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginInteraction();
	void EndInteraction();

	UGrabberComponent* GetGrabber() { return Grabber; }

protected:
	virtual void BeginPlay() override;

private:
	bool DetectInteractable(FHitResult& OutHitResult);

	UPROPERTY(VisibleAnywhere, Category = "Interactor")
	UGrabberComponent* Grabber;

	UPROPERTY(EditAnywhere, Category = "Interactor")
	TEnumAsByte<ECollisionChannel> TraceChannel;	// 상호작용 가능 액터를 트레이스 할 때 사용할 채널

	UPROPERTY(EditAnywhere, Category="Interactor")
	float DetectHeight = 0.f;	// 상호작용 가능 액터 탐지 트레이스 시작 높이

	UPROPERTY(EditAnywhere, Category = "Interactor")
	float DetectDistance = 65.f;	// 상호작용 가능 액터 탐지 트레이스 거리

	bool IsInteracting = false;	// 현재 상호작용 중(키를 누르고 있는 중) 여부
	UInteractableComponent* CurrentInteractable;	// 현재 상호작용 중인 액터의 InteractableComponent
};