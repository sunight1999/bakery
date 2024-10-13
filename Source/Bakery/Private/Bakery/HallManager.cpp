// Fill out your copyright notice in the Description page of Project Settings.


#include "Bakery/HallManager.h"
#include "EngineUtils.h"

#include "General/BakeryGameMode.h"
#include "Characters/Customer.h"
#include "Hall/Table.h"
#include "Hall/Chair.h"

AHallManager* AHallManager::Instance = nullptr;

AHallManager::AHallManager()
{
	PrimaryActorTick.bCanEverTick = true;

	WaitingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WaitingPoint"));
	RootComponent = WaitingPoint;
}

AHallManager::~AHallManager()
{
	Instance = nullptr;
}

AHallManager* AHallManager::GetInstance(UWorld* World)
{
	// World 유효성 검사
	if (!World || World->IsPreviewWorld() || World->bIsTearingDown)
	{
		return nullptr;
	}

	if (!Instance)
	{
		for (TActorIterator<AHallManager> It(World); It; ++It)
		{
			if (!Instance)
			{
				Instance = *It;
			}
			else
			{
				It->Destroy();
			}
		}

		if (!Instance)
		{
			Instance = World->SpawnActor<AHallManager>();
		}
	}

	return Instance;
}

#if WITH_EDITOR
void AHallManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	EnsureSingleton();
}

void AHallManager::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	EnsureSingleton();
}

void AHallManager::EnsureSingleton()
{
	if (Instance && Instance != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("다수의 싱글톤 액터가 확인되어, 불필요한 액터를 제거합니다."));
		Destroy();
	}
}
#endif

void AHallManager::BeginPlay()
{
	Super::BeginPlay();

	// TODO: RefreshHall 호출 시점을 인테리어 수정을 완료 했을 때로 변경하고 OnBakeryPreOpened는 제거
	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	BakeryGameMode->OnBakeryPreOpened.AddUObject(this, &AHallManager::RefreshHall);
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

	UE_LOG(LogTemp, Display, TEXT("TABLES : %d"), Tables.Num());
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
