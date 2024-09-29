// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/InteractableActor.h"
#include "Table.generated.h"

class ACustomer;
class AChair;

/**
 * 
 */
UCLASS()
class BAKERY_API ATable : public AInteractableActor
{
	GENERATED_BODY()

public:
	ATable();
	void BeginPlay() override;

	int GetEmptySeatNum() const { return EmptySeats.Num(); }

	AChair* RequestSeat();
	void LeaveSeat(AChair* Seat);

	void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	void OnInteract() override {}
	void OnExitInteract() override {}

private:
	UPROPERTY(VisibleAnywhere, Category = "Table")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Table")
	TSubclassOf<AChair> ChairClass;

	UPROPERTY(EditAnywhere, Category = "Table")
	float ChairDistance = 15.f;

	TArray<AChair*> EmptySeats;
	TSet<AChair*> UsingSeats;
};
