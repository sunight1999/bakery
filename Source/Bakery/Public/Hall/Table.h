// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Table.generated.h"

class ACustomer;
class AChair;
class ADish;
class UWidgetComponent;
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

	int GetEmptySeatNum() const { return EmptySeats.Num(); }

	AChair* RequestSeat();
	void LeaveSeat(AChair* Seat);

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	void OnInteract(float DeltaTime) override {}
	void OnExitInteract() override {}

	void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	void OnGrab(float DeltaTime) override {}
	void OnExitGrab() override {}

	void PutDish(ADish* Dish, AChair* Chair);

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

	ABakeryGameMode* BakeryGameMode;

	TArray<AChair*> EmptySeats;
	TSet<AChair*> UsingSeats;

	TArray<ADish*> ServedDishes;
};
