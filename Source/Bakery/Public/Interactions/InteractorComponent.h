// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractorComponent.generated.h"

class IInteract;
class IGrab;
class UGrabberComponent;

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

	// 상호작용 가능 액터를 트레이스 할 때 사용할 채널
	UPROPERTY(EditAnywhere, Category="Interaction")
	TEnumAsByte<ECollisionChannel> InteractTraceChannel;

	// 잡기 가능 액터를 트레이스 할 때 사용할 채널
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> GrabTraceChannel;

	// 상호작용 가능 액터 탐지 트레이스 시작 높이
	UPROPERTY(EditAnywhere, Category="Interaction")
	float DetectHeight = 0.f;

	// 상호작용 가능 액터 탐지 트레이스 거리
	UPROPERTY(EditAnywhere, Category="Interaction")
	float DetectDistance = 65.f;

	// 상호작용 가능 액터 존재 여부 확인 결과 (하이라이팅 처리용)
	AActor* PrevDetectedActor;

	// 현재 상호작용 중인 액터
	IInteract* CurrentInteracting;

	// 현재 잡기 중인 액터
	IGrab* CurrentGrabbing;
};
