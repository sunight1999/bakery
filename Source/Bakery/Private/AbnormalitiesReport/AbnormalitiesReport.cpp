// Fill out your copyright notice in the Description page of Project Settings.


#include "AbnormalitiesReport/AbnormalitiesReport.h"

// Sets default values
AAbnormalitiesReport::AAbnormalitiesReport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAbnormalitiesReport::OnCooldown()
{
}

void AAbnormalitiesReport::ShowCooldownMessage()
{

}

// Called when the game starts or when spawned
void AAbnormalitiesReport::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("TEsTTT"));

	GetWorld()->GetTimerManager().SetTimer(SituationCountHandle, this, &AAbnormalitiesReport::Choice, CooldownTime, true);
	
}

// Called every frame
void AAbnormalitiesReport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbnormalitiesReport::TimeCount(float Time)
{
	float CountDownTime = Time;
	
}
void AAbnormalitiesReport::TestLog(int Choice1) {
	UE_LOG(LogTemp, Display, TEXT("%d번 상황 발생!!!"), Choice1);

}

void AAbnormalitiesReport::Choice()
{
	SituationChoice = FMath::RandRange(1, Situation);
	AAbnormalitiesReport::TestLog(SituationChoice);
}
