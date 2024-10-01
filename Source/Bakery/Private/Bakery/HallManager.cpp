// Fill out your copyright notice in the Description page of Project Settings.


#include "Bakery/HallManager.h"
#include "EngineUtils.h"

#include "Characters/Customer.h"
#include "Hall/Table.h"
#include "Hall/Chair.h"

AHallManager::AHallManager()
{
	PrimaryActorTick.bCanEverTick = true;

	WaitingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WaitingPoint"));
	RootComponent = WaitingPoint;
}

void AHallManager::BeginPlay()
{
	Super::BeginPlay();
	
	RefreshHall();
}

void AHallManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleHall();
}

void AHallManager::HandleHall()
{
	if (WaitingQueue.Num() > 0 && GetEmptySeatNum() > 0)
	{
		AChair* EmptySeat = RequestSeat();
		if (!EmptySeat)
		{
			return;
		}

		// TODO: Array의 Pop 같은 경우 원소의 이동이 많이 일어나 비효율적, 개선안(Iteratable Queue 등) 필요
		ACustomer* Customer = WaitingQueue.Pop();
		Customer->SitTo(EmptySeat);

		// 빠진 자리를 채우기 위해 대기 위치 조정
		for (int i = 0; i < WaitingQueue.Num(); i++)
		{
			ACustomer* WaitingCustomer = WaitingQueue[i];
			FVector TargetPosition = CalulateWaitingPosition(i);

			WaitingCustomer->MoveTo(TargetPosition);
		}
	}
}

FVector AHallManager::CalulateWaitingPosition(int WaitingIndex)
{
	if (WaitingIndex == -1)
	{
		WaitingIndex = WaitingQueue.Num() - 1;
	}

	FVector Direction = bWaitingToTheRight ? FVector::RightVector : FVector::LeftVector;
	FVector Distance = WaitingIndex * WaitingDistance * Direction;
	return WaitingPoint->GetComponentLocation() + Distance;
}

/// <summary>
/// 자릿수, 좌석 위치 등 홀 정보를 갱신한다.
/// </summary>
void AHallManager::RefreshHall()
{
	Tables.Empty();
	TopPriorityTableIndex = 0;

	for (TActorIterator<ATable> It(GetWorld()); It; ++It)
	{
		Tables.Emplace(*It);
	}
}

int AHallManager::GetEmptySeatNum()
{
	int EmptySeatNum = 0;

	for (ATable* Table : Tables)
	{
		EmptySeatNum += Table->GetEmptySeatNum();
	}

	return EmptySeatNum;
}

/// <summary>
/// 빈 자리를 요청한다.
/// </summary>
/// <returns>손님에게 배정된 자리. 없을 경우 nullptr</returns>
AChair* AHallManager::RequestSeat()
{
	int LeftTableNum = Tables.Num();

	while (LeftTableNum--)
	{
		if (TopPriorityTableIndex == Tables.Num())
		{
			TopPriorityTableIndex = 0;
		}

		ATable* Table = Tables[TopPriorityTableIndex++];
		AChair* Chair = Table->RequestSeat();

		if (Chair)
		{
			return Chair;
		}
	}

	return nullptr;
}

/// <summary>
/// 대기를 요청한다.
/// </summary>
/// <param name="Customer"></param>
/// <returns>손님이 대기해야 하는 자리</returns>
FVector AHallManager::RequestWaiting(ACustomer* Customer)
{
	WaitingQueue.Emplace(Customer);

	return CalulateWaitingPosition();
}
