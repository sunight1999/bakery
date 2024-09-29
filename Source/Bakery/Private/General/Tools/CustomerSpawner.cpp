// Fill out your copyright notice in the Description page of Project Settings.


#include "General/Tools/CustomerSpawner.h"
#include "EngineUtils.h"

#include "Bakery/HallManager.h"
#include "Subsystems/RecipeSubsystem.h"
#include "Characters/Customer.h"
#include "Hall/Chair.h"

ACustomerSpawner::ACustomerSpawner()
{

}

void ACustomerSpawner::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AHallManager> It(World); It; ++It)
	{
		HallManager = *It;
		break;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	RecipeSubsystem = GameInstance->GetSubsystem<URecipeSubsystem>();
}

void ACustomerSpawner::PostSpawn(AActor* Actor)
{
	ACustomer* Customer = Cast <ACustomer>(Actor);
	if (!Customer)
	{
		return;
	}

	Customer->SetHallManager(HallManager);

	// TODO: 랜덤으로 레시피 중 하나 지정
	Customer->SetOrder(RecipeSubsystem->GetRecipe(FName("Injeolmi")));

	// 홀에 자리가 있을 경우 지정된 좌석으로 Customer 이동
	AChair* Seat = HallManager->RequestSeat();
	if (Seat)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *Seat->GetActorNameOrLabel());
		Customer->SitTo(Seat);
	}
	// 홀에 자리가 없을 경우 지정된 대기 위치로 Customer 이동
	else
	{
		FVector WaitingPosition = HallManager->RequestWaiting(Customer);
		Customer->MoveTo(WaitingPosition);
	}
}
