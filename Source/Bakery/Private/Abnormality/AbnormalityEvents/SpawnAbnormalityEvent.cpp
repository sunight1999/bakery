// Fill out your copyright notice in the Description page of Project Settings.


#include "Abnormality/AbnormalityEvents/SpawnAbnormalityEvent.h"

void ASpawnAbnormalityEvent::Cause()
{
	Super::Cause();

	UE_LOG(LogTemp, Display, TEXT("Hello"));
}

void ASpawnAbnormalityEvent::Finish()
{
	Super::Finish();
}
