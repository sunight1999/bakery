// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DessertSceneCapturer.generated.h"

UCLASS()
class BAKERY_API ADessertSceneCapturer : public AActor
{
	GENERATED_BODY()
	
public:	
	ADessertSceneCapturer();

	void SetDessertMesh(UStaticMesh* Mesh);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "DessertSceneCapturer")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "DessertSceneCapturer")
	USceneComponent* DessertScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DessertSceneCapturer", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DessertMesh;

	UPROPERTY(EditAnywhere, Category = "DessertSceneCapturer")
	TMap<UStaticMesh*, float> ScalingMap;
};
