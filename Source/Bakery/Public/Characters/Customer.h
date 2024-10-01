// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "../Interactions/Interactables/Interact.h"
#include "Customer.generated.h"

class AHallManager;
class UInteractableComponent;
class UWidgetComponent;
class UAnimMontage;
class AAIController;
class AChair;
class AIngredient;
class URecipeData;
class UDishWaitingTimeBarWidget;

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
	void SetOrder(const URecipeData* RecipeData);
	void SetHallManager(AHallManager* InHallManager) { HallManager = InHallManager; }
	ECustomerState GetState() const { return CustomerState; }
	float GetRemainingWaitingTime() const { return TimerManager->GetTimerRemaining(WaitingTimer); }
	float GetWaitingTime() const { return TimerManager->GetTimerRate(WaitingTimer); }

	/*
	 * 상태별 처리 함수
	 */
	void TakeOrder();

private:
	void HandleIdle();
	void HandleSitting();
	void RequestTakeOrder();
	void OrderDish();
	void Eat(AIngredient* Dish);
	void FinishEating();
	void Leave();

	/*
	 * 주문 처리 관련 함수
	 */
public:
	FORCEINLINE void SetWaitingTimer(float WaitingTime);
	FORCEINLINE void ClearWaitingTimer();
	bool ServeDish(AIngredient* Dish);
	void Disappoint();

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
	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) {}

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract() override {}

	UFUNCTION(BlueprintCallable)
	virtual void OnExitInteract() override {}

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Customer");
	UInteractableComponent* Interactable;

	UPROPERTY(VisibleAnywhere, Category = "Customer");
	UWidgetComponent* DishWaitingTimeBarWidget;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float OrderWaitingTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float DishWaitingTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float EatingTime = 10.f;

	/*
	 * AI 및 이동 관련
	 */
	AAIController* CustomerController;
	ECustomerState CustomerState = ECustomerState::Idle;

	AHallManager* HallManager = nullptr;
	AChair* AssignedSeat = nullptr;
	FVector TargetPosition;

	/*
	 * 주문 및 식사 관련
	 */
	const URecipeData* Order = nullptr;
	AIngredient* ServedDish = nullptr;

	FTimerManager* TimerManager;
	FTimerHandle WaitingTimer;
	FTimerHandle EatingTimer;

	UDishWaitingTimeBarWidget* DishWaitingTimeBar;
};
