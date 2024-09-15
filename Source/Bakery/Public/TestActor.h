// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "TestActor.generated.h"

UCLASS()
class BAKERY_API ATestActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	ATestActor();
	virtual void Tick(float DeltaTime) override;

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	void OnInteract() override;
	void OnExitInteract() override;

protected:
	virtual void BeginPlay() override;

private:

};
