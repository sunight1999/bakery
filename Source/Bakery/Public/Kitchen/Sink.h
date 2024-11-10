// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Sink.generated.h"

class ADish;

/**
 * 
 */
UCLASS()
class BAKERY_API ASink : public AGrabInteractableActor
{
	GENERATED_BODY()

public:
	ASink();

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	virtual void OnInteract() override {}
	virtual void OnExitInteract() override {}

	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	virtual void OnGrab() override {}
	virtual void OnExitGrab() override {}
	
private:
	FVector CalculateDishLocation(int Index);

	/*
	 * 메쉬 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Sink")
	UStaticMeshComponent* WaterMesh;

	UPROPERTY(VisibleAnywhere, Category = "Sink")
	UStaticMeshComponent* TopMesh;

	UPROPERTY(VisibleAnywhere, Category = "Sink")
	UStaticMeshComponent* BottomMesh;

	/*
	 * 접시 보관 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Sink")
	USceneComponent* DishKeepingBeginPoint;

	UPROPERTY(EditAnywhere, Category = "Sink")
	bool bKeepingToTheRight = true;

	UPROPERTY(EditAnywhere, Category = "Sink")
	int MaxDishNum = 10;

	UPROPERTY(EditAnywhere, Category = "Sink")
	float KeepingDishDistance = 20.f;

	TArray<ADish*> KeepingDishes;
};
