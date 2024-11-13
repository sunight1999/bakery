// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chair.generated.h"

class ATable;
class ACustomer;

UCLASS()
class BAKERY_API AChair : public AActor
{
	GENERATED_BODY()
	
public:	
	AChair();
	void Tick(float DeltaTime) override;

	void Initialize();
	void PullOutChair();
	void PutInChair();

	/*
	 * Getter & Setter Functions
	 */
	float GetWaitingMultipler() const { return WaitingTimeMultiplier; }

	ATable* GetOwnerTable();
	void SetOwnerTable(ATable* Table) { OwnerTable = Table; }

	ACustomer* GetAssignedCustomer() const { return AssignedCustomer; }
	void SetAssignedCustomer(ACustomer* Customer) { AssignedCustomer = Customer; }

	const FVector& GetChairDirection() const { return ChairDirection; }
	void SetChairDirection(const FVector& Direction) { ChairDirection = Direction; }

	float GetStopToSitDistance() const { return StopToSitDistance; }
	float GetPullOutDistance() const { return PullOutDistance; }

	FVector GetStopToSitPointLocation() const { return GetActorLocation() + GetActorRightVector() * StopToSitDistance + GetActorForwardVector() * -StopToSitDistance; }
	FVector GetSitPointLocation() const { return SitPoint->GetComponentLocation(); }
	FVector GetSitPointRelativeLocation() const { return SitPoint->GetRelativeLocation(); }
	FRotator GetSitPointRotation() const { return SitPoint->GetComponentRotation(); }
	FRotator GetSitPointRelativeRotation() const { return SitPoint->GetComponentRotation(); }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Chair")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Chair")
	USceneComponent* SitPoint;

	UPROPERTY(EditAnywhere, Category = "Chair")
	float StopToSitDistance = 10.f;

	UPROPERTY(EditAnywhere, Category = "Chair")
	float PullOutDistance = 30.f;

	UPROPERTY(EditAnywhere, Category = "Chair")
	float PullOutSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category="Chair|Attribute")
	float WaitingTimeMultiplier = 1.f;

	ATable* OwnerTable = nullptr;
	ACustomer* AssignedCustomer = nullptr;

	FVector ChairDirection;

	bool bIsMoving = false;
	FVector MovingTargetLocation;
	FVector PullOutLocation;
};
