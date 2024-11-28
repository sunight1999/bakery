// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CookingStateIndicator.generated.h"

enum class ECookingIndicator : uint8
{
	Done,
	Failed,
	Target
};

class UFloatingComponent;

UCLASS()
class BAKERY_API ACookingStateIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ACookingStateIndicator();
	void Initailize(const FVector& Location, const FVector& Scale);

	void Show(ECookingIndicator CookingIndicator);
	void Hide();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Indicator")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Indicator")
	UStaticMeshComponent* CookingDoneIconMesh;

	UPROPERTY(VisibleAnywhere, Category = "Indicator")
	UStaticMeshComponent* CookingFailedIconMesh;

	UPROPERTY(VisibleAnywhere, Category = "Indicator")
	UStaticMeshComponent* CookingNextTargetIconMesh;

	UPROPERTY(VisibleAnywhere, Category="Indicator")
	UFloatingComponent* FloatingComponent;

	FVector OriginLocation;
	FVector OriginScale;
};
