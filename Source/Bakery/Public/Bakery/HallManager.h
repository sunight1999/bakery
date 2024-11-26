// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HallManager.generated.h"

class ACustomer;
class ATable;
class AChair;
class ABakeryGameState;

UCLASS()
class BAKERY_API AHallManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHallManager();
	~AHallManager();
	static AHallManager* GetInstance(UWorld* World);

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void PostEditMove(bool bFinished);
	void EnsureSingleton();
#endif

	virtual void Tick(float DeltaTime) override;

	/*
	 * 좌석 배치 관련 함수
	 */
	void RefreshHall();
	int GetEmptySeatNum();

	AChair* RequestSeat();
	FVector RequestWaiting(ACustomer* Customer);

	/*
	 * 영업 정산 관련 함수
	 */
	void SettleSales();
	void AddPendingMoney(int InMoney) { PendingMoney += InMoney; }
	void AddPendingRating(int InRating) { PendingRating = PendingRating == 0 ? InRating : (PendingRating + InRating) / 2.f; }

protected:
	virtual void BeginPlay() override;

private:
	static AHallManager* Instance;

	void HandleHall();

	FVector CalulateWaitingPosition(int WaitingIndex = -1);

	/*
	 * 좌석 배치 관련
	 */
	// 대기열 시작 위치
	UPROPERTY(EditDefaultsOnly, Category="Hall")
	USceneComponent* WaitingPoint;

	// 대기열을 오른쪽으로 늘어뜨릴지 왼쪽으로 늘어뜨릴지 여부
	UPROPERTY(EditDefaultsOnly, Category = "Hall")
	bool bWaitingToTheRight = true;

	// 대기열 간격
	UPROPERTY(EditDefaultsOnly, Category = "Hall")
	float WaitingDistance = 100.f;

	TArray<ACustomer*> WaitingQueue;

	TArray<ATable*> Tables;
	int TopPriorityTableIndex = 0;

	/*
	 * 영업 정산 관련
	 */
	ABakeryGameState* BakeryGameState = nullptr;

	int PendingMoney = 0;
	float PendingRating = 0.f;
};
