// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "../General/BakeryDefines.h"
#include "Customer.generated.h"

enum class EWaitingState : uint8;
class AHallManager;
class UWidgetComponent;
class UAnimMontage;
class AAIController;
class AChair;
class AIngredient;
class ADish;
class URecipeData;
class UCustomerStateWidget;

UCLASS()
class BAKERY_API ACustomer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ACustomer();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnCusotmerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/*
	 * Getter & Setter
	 */
	const URecipeData* GetOrder() const { return Order; }
	void SetOrder(const URecipeData* RecipeData);

	UFUNCTION(BlueprintCallable)
	ECustomerState GetState() const { return CustomerState; }

	float GetRemainingWaitingTime() const { return TimerManager->GetTimerRemaining(WaitingTimer); }
	float GetWaitingTime() const { return TimerManager->GetTimerRate(WaitingTimer); }

	void SetDespawnPosition(const FVector& Position) { DespawnPosition = Position; }

	/*
	 * 상태별 처리 함수 (외부 액터에 의해 호출 또는 상태 변경)
	 */
	UFUNCTION()
	void StateChanged(EWaitingState NewState);

	UFUNCTION(BlueprintCallable)
	void RequestTakeOrder();
	void TakeOrder();
	bool ServeDish(ADish* Dish);
	void Disappoint();
	UFUNCTION(BlueprintCallable)
	void Leave();

	void LoseDish(float InMoveSpeed, float InFlySpeed, float InAmplitude, float InMoveInterval);
	void RegainDish();

private:
	/*
	 * 상태별 처리 함수
	 */
	void HandleIdle();
	void OrderDish();
	void Eat(ADish* Dish);
	void FinishEating();

public:
	/*
	 * 주문 대기 UI 관련 함수
	 */
	void PauseTimers();
	void ResumeTimers();
	FORCEINLINE void SetWaitingTimer(FString WaitingIconPath, float WaitingTime);
	FORCEINLINE void ClearWaitingTimer();

	/*
	 * 이동 관련 함수
	 */
	void MoveTo(const FVector& InTargetPosition);
	void MoveToDespawn();
	void SitTo(AChair* Seat);
	bool IsInTargetPosition(const float ToleranceRadius = 10.f) const;

	/*
	 * 이상현상 관련 함수
	 */
	bool IsFeared() const { return bIsFeared; }
	void SetFeared(bool bFeared);

	bool IsDishBelongs(ADish* Dish) { return Dish == ServedDish; }

	void ClearFearWidget();

protected:
	virtual void BeginPlay() override;

private:
	/*
	 * 주문 대기 및 식사 시간 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Customer");
	UWidgetComponent* CustomerStateWidget;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float OrderWaitingTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float DishWaitingTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float EatingTime = 10.f;

	const URecipeData* Order = nullptr;
	ADish* ServedDish = nullptr;

	FTimerManager* TimerManager;
	FTimerHandle WaitingTimer;
	FTimerHandle EatingTimer;

	UCustomerStateWidget* CustomerStateUI;

	// 평점. 평점은 대기시간이 Bad 상태까지 넘어가거나, 공포도가 Bad 상태까지 넘어가면 -1. 둘 중 하나라도 초과할 경우 최소치인 1 부여.
	int Rating = 5;

	/*
	 * AI 및 이동 관련
	 */
	UPROPERTY(EditAnywhere, Category = "Customer");
	float AcceptanceRadius = 10.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float PlayerPushForce = 1.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float SitMovementSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float SitRotationSpeed = 5.f;

	AAIController* CustomerController;
	ECustomerState CustomerState = ECustomerState::Idle;

	AChair* AssignedSeat = nullptr;
	FVector MoveTargetPosition;
	FTransform SitTargetTransform;
	FVector DespawnPosition;

	/*
	 * 이상현상 관련
	 */
	UPROPERTY(EditAnywhere, Category = "Customer")
	UAnimMontage* CustomerMontage;

	UPROPERTY(EditAnywhere, Category = "Customer")
	float MaxFearedTime = 15.f;
	float CurrentFearedTime = 0.f;
	bool bIsFeared = false;
};
