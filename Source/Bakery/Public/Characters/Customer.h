// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "../Interactions/Interactables/Interact.h"
#include "Customer.generated.h"

class AHallManager;
class UInteractableComponent;
class UAnimMontage;
class AAIController;
class AChair;
class AIngredient;
class URecipeData;

UCLASS()
class BAKERY_API ACustomer : public ABaseCharacter, public IInteract
{
	GENERATED_BODY()

public:
	ACustomer();
	virtual void Tick(float DeltaTime) override;

	/*
	 * Getter & Setter
	 */
	const URecipeData* GetOrder() const { return Order; }
	void SetOrder(const URecipeData* RecipeData) { Order = RecipeData; }
	void SetHallManager(AHallManager* InHallManager) { HallManager = InHallManager; }
	ECustomerState GetState() const { return CustomerState; }

	/*
	 * 상태별 처리 함수
	 */
private:
	void HandleIdle();
	void HandleSitting();
	void HandleOrdering();
	void HandleWaitingDish();
	void HandleEating();

	/*
	 * 주문 처리 관련 함수
	 */
public:
	bool ServeDish(AIngredient* Dish);

	/*
	 * 이동 관련 함수
	 */
	void MoveTo(const FVector& InTargetPosition);
	void SitTo(AChair* Seat);
	bool IsInTargetPosition(const float ToleranceRadius = 100.f) const;

	/*
	 * Interaction Functions
	 */
	UFUNCTION(BlueprintCallable)
	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract() override {}

	UFUNCTION(BlueprintCallable)
	virtual void OnExitInteract() override {}

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Customer");
	UInteractableComponent* Interactable;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float OrderWaitingTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float DishWaitingTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float EatingTime = 10.f;

	AAIController* CustomerController;
	ECustomerState CustomerState = ECustomerState::Idle;

	const URecipeData* Order = nullptr;
	
	AHallManager* HallManager = nullptr;
	AChair* AssignedSeat = nullptr;

	FVector TargetPosition;
};
