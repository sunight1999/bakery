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

	float GetWaitingMultipler() const { return WaitingTimeMultiplier; }
	ATable* GetOwnerTable();
	void SetOwnerTable(ATable* Table) { OwnerTable = Table; }
	ACustomer* GetAssignedCustomer() const { return AssignedCustomer; }
	void SetAssignedCustomer(ACustomer* Customer) { AssignedCustomer = Customer; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Chair")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Chair|Attribute")
	float WaitingTimeMultiplier = 1.f;

	ATable* OwnerTable = nullptr;
	ACustomer* AssignedCustomer = nullptr;
};
