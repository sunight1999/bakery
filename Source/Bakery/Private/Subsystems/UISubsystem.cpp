// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UISubsystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

UUISubsystem::UUISubsystem()
{
	/*
	ConstructorHelpers::FObjectFinder<UDataTable> UIDataTableFinder(TEXT("/Game/Data/UI/DT_ManagedUI"));
	if (UIDataTableFinder.Succeeded())
	{
		UIDataTable = UIDataTableFinder.Object;
	}

	FString ContextString = "ManagedUI";
	TArray<FManagedUI*> UIArray;
	UIDataTable->GetAllRows<FManagedUI>(ContextString, UIArray);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UE_LOG(LogTemp, Display, TEXT("NOT NULL"));
	}
	else
		UE_LOG(LogTemp, Display, TEXT("NULLNULLNULLNULLNULLNULLNULLNULLNULLNULLNULL"));

	for (FManagedUI* ManagedUI : UIArray)
	{
		GetWorld()->Createwid
	}
	*/
}

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
}

void UUISubsystem::SetUIVisibility(FName UIName, ESlateVisibility Visibility)
{
	// Set이면 현재 layer 업데이트 및 표시 + stack clear, Add면 stack으로 순서 관리
}

void UUISubsystem::ReverseUILayer()
{
	// add stack에서 하나씩 pop하며 invisible 처리
}
