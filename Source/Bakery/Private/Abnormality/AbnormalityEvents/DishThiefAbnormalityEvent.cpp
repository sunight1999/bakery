// Fill out your copyright notice in the Description page of Project Settings.


#include "Abnormality/AbnormalityEvents/DishThiefAbnormalityEvent.h"
#include "EngineUtils.h"

#include "Characters/Customer.h"

ADishThiefAbnormalityEvent::ADishThiefAbnormalityEvent()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADishThiefAbnormalityEvent::Tick(float DeltaTime)
{

}

void ADishThiefAbnormalityEvent::Cause()
{
	Super::Cause();

	GetWorld()->GetTimerManager().SetTimer(ThiefHandle, this, &ADishThiefAbnormalityEvent::Steal, 20.f, true, 0.f);
	bIsCaused = true;
}

void ADishThiefAbnormalityEvent::Finish()
{
	Super::Finish();

	GetWorld()->GetTimerManager().ClearTimer(ThiefHandle);
}

void ADishThiefAbnormalityEvent::Steal()
{
	for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
	{
		ACustomer* Customer = *It;
		if (!Customer->IsFeared() && Customer->GetState() == ECustomerState::Eating)
		{
			Customer->LoseDish(MoveSpeed, FlySpeed, Amplitude, MoveInterval);
			break;
		}
	}
}
