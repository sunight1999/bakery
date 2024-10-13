// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Table.generated.h"

class ACustomer;
class AChair;
class UWidgetComponent;
class UWaitingTimeBarWidget;

/**
 * 
 */
UCLASS()
class BAKERY_API ATable : public AGrabInteractableActor
{
	GENERATED_BODY()

public:
	ATable();
	void Tick(float DeltaTime) override;

	int GetEmptySeatNum() const { return EmptySeats.Num(); }

	AChair* RequestSeat();
	void LeaveSeat(AChair* Seat);
	void RequestTakeOrder(ACustomer* Customer);

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	void OnInteract() override {}
	void OnExitInteract() override {}
	void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	void OnGrab() override {}
	void OnExitGrab() override {}

private:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Table")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Table")
	TSubclassOf<AChair> ChairClass;

	UPROPERTY(EditAnywhere, Category = "Table")
	float ChairDistance = 15.f;

	UPROPERTY(VisibleAnywhere, Category = "Table")
	UWidgetComponent* WaitingTimeBarWidget;

	UWaitingTimeBarWidget* WaitingTimeBar;
	ACustomer* WaitingTrackedCustomer = nullptr;

	TArray<AChair*> EmptySeats;
	TSet<AChair*> UsingSeats;
};
