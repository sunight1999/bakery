// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cloner.generated.h"

enum class EDirection : uint8;
class UBoxComponent;
class UInstancedStaticMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;
struct FPrimitiveInstancedId;

UCLASS()
class BAKERY_API ACloner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACloner();
	void OnConstruction(const FTransform& Transform) override;
	void Initialize();

	const int GetLeftNum() const { return LeftNum; }
	const int GetRightNum() const { return RightNum; }
	const int GetForwardNum() const { return ForwardNum; }

	const float GetDistanceX() const { return DistanceX; }
	const float GetDistanceY() const { return DistanceY; }
	const float GetDistanceZ() const { return DistanceZ; }
	
	//void Expand(EDirection Direction);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UInstancedStaticMeshComponent* ISM;

	UPROPERTY(EditAnywhere, Category = "Cloner")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Cloner", meta=(UIMin="0", UIMax="20"))
	uint32 LeftNum = 1;

	UPROPERTY(EditAnywhere, Category="Cloner", meta = (UIMin = "0", UIMax = "20"))
	uint32 RightNum = 1;

	UPROPERTY(EditAnywhere, Category = "Cloner", meta = (UIMin = "0", UIMax = "20"))
	uint32 ForwardNum = 1;

	UPROPERTY(EditAnywhere, Category = "Cloner", meta = (UIMin = "0", UIMax = "20"))
	uint32 ZNum = 1;


	UPROPERTY(EditAnywhere, Category = "Cloner")
	float StartOffsetX = 100.f;

	UPROPERTY(EditAnywhere, Category = "Cloner")
	float StartOffsetY = 100.f;

	UPROPERTY(EditAnywhere, Category = "Cloner")
	float StartOffsetZ = 0.f;

	UPROPERTY(EditAnywhere, Category = "Cloner")
	float DistanceX = 200.f;

	UPROPERTY(EditAnywhere, Category = "Cloner")
	float DistanceY = 200.f;

	UPROPERTY(EditAnywhere, Category = "Cloner")
	float DistanceZ = 200.f;
};
