// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractorComponent.generated.h"

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

protected:
	virtual void BeginPlay() override;

private:
	bool DetectInteractable(FHitResult& OutHitResult);

	UPROPERTY(EditAnywhere, Category = "Interactor")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, Category="Interactor")
	float DetectHeight = 0.f;

	UPROPERTY(EditAnywhere, Category = "Interactor")
	float DetectDistance = 65.f;

	bool IsInteracting = false;
	UInteractableComponent* CurrentInteractable;
};
