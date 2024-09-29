// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HallManager.generated.h"

class ACustomer;
class ATable;
class AChair;

UCLASS()
class BAKERY_API AHallManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHallManager();
	virtual void Tick(float DeltaTime) override;

	void RefreshHall();
	int GetEmptySeatNum();

	AChair* RequestSeat();
	FVector RequestWaiting(ACustomer* Customer);

protected:
	virtual void BeginPlay() override;

private:
	void HandleHall();

	FVector CalulateWaitingPosition(int WaitingIndex = -1);

	/*
	 * 대기열 관련
	 */
	UPROPERTY(EditDefaultsOnly, Category="Hall")
	USceneComponent* WaitingPoint;	// 대기열 시작 위치

	UPROPERTY(EditDefaultsOnly, Category = "Hall")
	bool bWaitingToTheRight = true;	// 대기열을 오른쪽으로 늘어뜨릴지 왼쪽으로 늘어뜨릴지 여부

	UPROPERTY(EditDefaultsOnly, Category = "Hall")
	float WaitingDistance = 100.f;	// 대기열 간격

	TArray<ACustomer*> WaitingQueue;

	/*
	 * 홀 관련
	 */
	TArray<ATable*> Tables;

	int TopPriorityTableIndex = 0;
};
