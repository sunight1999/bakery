// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Countertop.generated.h"

/**
 * 재료나 요리, 요리 도구 등을 올려둘 수 있는 조리대
 * - 썰기 등 기본적인 손질 가능
 */
UCLASS()
class BAKERY_API ACountertop : public AGrabInteractableActor
{
	GENERATED_BODY()
	
public:
	ACountertop();
	virtual void Tick(float DeltaTime) override;

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	virtual void OnGrab() override;
	virtual void OnExitGrab() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Remodel|Countertop")
	UStaticMeshComponent* OptionalToolMesh;

	UPROPERTY(VisibleAnywhere, Category = "Remodel|Countertop")
	UStaticMeshComponent* TopPlateMesh;

	UPROPERTY(VisibleAnywhere, Category = "Remodel|Countertop")
	UStaticMeshComponent* BottomSupportMesh;

	UPROPERTY(VisibleAnywhere, Category = "Remodel|Countertop")
	USceneComponent* KeepPoint;

	AActor* CurrentKeptActor;
};
