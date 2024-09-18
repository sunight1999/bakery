// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractorComponent.generated.h"

class UGrabberComponent;
class UInteractableComponent;

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Interact,
	Grab
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKERY_API UInteractorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginInteraction();
	void EndInteraction();

	void BeginGrab();
	void EndGrab();

	UGrabberComponent* GetGrabber() { return Grabber; }

protected:
	virtual void BeginPlay() override;

private:
	bool DetectInteractable(const TEnumAsByte<ECollisionChannel>& TraceChannel, FHitResult& OutHitResult);

	UPROPERTY(VisibleAnywhere, Category="Interaction|Grabber")
	UGrabberComponent* Grabber;

	UPROPERTY(EditAnywhere, Category="Interaction")
	TEnumAsByte<ECollisionChannel> InteractTraceChannel;	// 상호작용 가능 액터를 트레이스 할 때 사용할 채널

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> GrabTraceChannel;	// 잡기 가능 액터를 트레이스 할 때 사용할 채널

	UPROPERTY(EditAnywhere, Category="Interaction")
	float DetectHeight = 0.f;	// 상호작용 가능 액터 탐지 트레이스 시작 높이

	UPROPERTY(EditAnywhere, Category="Interaction")
	float DetectDistance = 65.f;	// 상호작용 가능 액터 탐지 트레이스 거리


	bool bIsInteracting = false;	// 현재 상호작용 중(키를 누르고 있는 중) 여부
	bool bIsGrabbing = false; // 현재 잡기 중 여부
	UInteractableComponent* CurrentInteractable;	// 현재 상호작용 중인 액터의 InteractableComponent
};
