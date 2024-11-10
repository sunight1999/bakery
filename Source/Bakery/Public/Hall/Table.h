// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Table.generated.h"

class ACustomer;
class AChair;
class ADish;
class UWidgetComponent;
class UWaitingTimeBarWidget;
class ABakeryGameMode;

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

	UPROPERTY(VisibleAnywhere, Category = "Table")
	USceneComponent* DishServingCenterPoint;

	UPROPERTY(EditAnywhere, Category="Table")
	TSubclassOf<AChair> ChairClass;

	UPROPERTY(EditAnywhere, Category = "Table")
	float ChairDistance = 15.f;

	UPROPERTY(EditAnywhere, Category = "Table")
	float DishesDistance = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "Table")
	UWidgetComponent* WaitingTimeBarWidget;

	UWaitingTimeBarWidget* WaitingTimeBar;
	ACustomer* WaitingTrackedCustomer = nullptr;

	ABakeryGameMode* BakeryGameMode;

	TArray<AChair*> EmptySeats;
	TSet<AChair*> UsingSeats;

	TArray<ADish*> ServedDishes;
};
