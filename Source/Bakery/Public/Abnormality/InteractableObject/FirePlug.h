
#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "FirePlug.generated.h"

/**
 * 소화전
 */
class UProgressWidget;
class UWidgetComponent;
class ACookingStateIndicator;

UCLASS()
class BAKERY_API AFirePlug : public AInteractableActor
{
	GENERATED_BODY()

public:
	AFirePlug();

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	void OnInteract(float deltatime) override;
	void OnExitInteract()override;

	void SetEventFlag(bool Flag);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "FirePlug")
	UStaticMeshComponent* FirePlugMesh;

	UPROPERTY(VisibleAnywhere, Category = "FirePlug")
	UWidgetComponent* ProgressComponent;
	UProgressWidget* ActivationProgress;

	UPROPERTY(EditAnywhere, Category = "FirePlug")
	float MaxGauge = 5.f;
	float CurrentGauge = 0;

	UPROPERTY(EditAnywhere, Category = "FirePlug")
	TSubclassOf<ACookingStateIndicator> StateIndicatorClass;

	UPROPERTY(VisibleAnywhere, Category = "FirePlug")
	USceneComponent* StateIndicatorPoint;

	UPROPERTY(EditAnywhere, Category = "FirePlug")
	FRotator StateFixedRotation = FRotator::ZeroRotator;

	ACookingStateIndicator* StateIndicator;

	bool EventFlag = false;
};
