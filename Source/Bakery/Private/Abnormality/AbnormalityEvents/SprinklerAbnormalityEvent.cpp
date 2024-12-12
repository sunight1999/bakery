// Fill out your copyright notice in the Description page of Project Settings.

#include "Abnormality/AbnormalityEvents/SprinklerAbnormalityEvent.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"

#include "Abnormality/InteractableObject/FirePlug.h"

void ASprinklerAbnormalityEvent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < SpawnRowNum; i++)
	{
		FVector RowOffset = FVector::BackwardVector * (i * SpawnRowDistance);
		for (int j = 0; j < SpawnColumnNum; j++)
		{
			FVector ColumnOffset = FVector::RightVector * (j * SpawnColumnDistance);
			FVector Location = SprinklerSpawnLocation + RowOffset + ColumnOffset;
			AActor* Sprinkler = GetWorld()->SpawnActor(SprinklerActorClass);
			Sprinkler->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Sprinkler->SetActorLocation(Location);
			Sprinkler->SetActorHiddenInGame(true);

			Sprinklers.Emplace(Sprinkler);
		}
	}
}

void ASprinklerAbnormalityEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASprinklerAbnormalityEvent::Cause()
{
	Super::Cause();

	// 스프링클러 활성화
	for (AActor* Sprinkler : Sprinklers)
	{
		if (UNiagaraComponent* Niagara = Sprinkler->GetComponentByClass<UNiagaraComponent>())
		{
			Sprinkler->SetActorHiddenInGame(false);
			Niagara->Activate();
		}
	}

	// 소화전 활성화
	for (TActorIterator<AFirePlug> It(GetWorld()); It; ++It)
	{
		(*It)->SetEventFlag(true);
	}
}

void ASprinklerAbnormalityEvent::Finish()
{
	Super::Finish();

	for (AActor* Sprinkler : Sprinklers)
	{
		if (UNiagaraComponent* Niagara = Sprinkler->GetComponentByClass<UNiagaraComponent>())
		{
			Sprinkler->SetActorHiddenInGame(true);
			Niagara->Deactivate();
		}
	}
}
