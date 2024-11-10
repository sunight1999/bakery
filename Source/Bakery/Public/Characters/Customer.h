// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "../General/BakeryDefines.h"
#include "Customer.generated.h"

class AHallManager;
class UWidgetComponent;
class UAnimMontage;
class AAIController;
class AChair;
class AIngredient;
class ADish;
class URecipeData;
class UDishWaitingTimeBarWidget;

UCLASS()
class BAKERY_API ACustomer : public ABaseCharacter
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
	ECustomerState GetState() const { return CustomerState; }
	float GetRemainingWaitingTime() const { return TimerManager->GetTimerRemaining(WaitingTimer); }
	float GetWaitingTime() const { return TimerManager->GetTimerRate(WaitingTimer); }
	void SetDespawnPosition(const FVector& Position) { DespawnPosition = Position; }

	/*
	 * 상태별 처리 함수
	 */
	void TakeOrder();

private:
	void HandleIdle();
	void HandleSitting();
	void RequestTakeOrder();
	void OrderDish();
	void Eat(ADish* Dish);
	void FinishEating();

public:
	void Leave();

	/*
	 * 주문 처리 관련 함수
	 */
	FORCEINLINE void SetWaitingTimer(float WaitingTime);
	FORCEINLINE void ClearWaitingTimer();
	bool ServeDish(ADish* Dish);
	void Disappoint();

	/*
	 * 이동 관련 함수
	 */
	void MoveTo(const FVector& InTargetPosition);
	void SitTo(AChair* Seat);
	bool IsInTargetPosition(const float ToleranceRadius = 100.f) const;

protected:
	virtual void BeginPlay() override;

private:
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

	AChair* AssignedSeat = nullptr;
	FVector TargetPosition;

	FVector DespawnPosition;

	/*
	 * 주문 및 식사 관련
	 */
	const URecipeData* Order = nullptr;
	ADish* ServedDish = nullptr;

	FTimerManager* TimerManager;
	FTimerHandle WaitingTimer;
	FTimerHandle EatingTimer;

	UDishWaitingTimeBarWidget* DishWaitingTimeBar;
};
