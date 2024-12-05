// Copyright Epic Games, Inc. All Rights Reserved.

#include "General/BakeryGameMode.h"
#include "Characters/BakeryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "TimerManager.h"

#include "General/BakeryGameState.h"
#include "Subsystems/UISubsystem.h"
#include "Characters/BakeryPlayerController.h"
#include "Characters/Customer.h"
#include "Widgets/HUD/BakeryHUDWidget.h"
#include "General/Tools/CustomerSpawner.h"
#include "Abnormality/AbnormalityManager.h"
#include "Subsystems/SoundManager.h"
#include "Kitchen/IngredientContainer.h"
#include "Kitchen/DishContainer.h"

ABakeryGameMode::ABakeryGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABakeryGameMode::BeginPlay()
{
	Super::BeginPlay();

	BakeryGameState = Cast<ABakeryGameState>(GameState);
	//FTimerHandle UILoadHandle;
	//GetWorld()->GetTimerManager().SetTimer(UILoadHandle, this, &ABakeryGameMode::LoadUI, 1.f, false);

	PlayerController = Cast<ABakeryPlayerController>(GetWorld()->GetFirstPlayerController());

	for (TActorIterator<ACustomerSpawner> It(GetWorld()); It; ++It)
	{
		CustomerSpawners.Add(*It);
	}

	// 게임 시간 설정
	float TimeTickRate = 1.f / GameTimeMultiplier;
	BakeryGameState->SetTime(GameStartTime);
	GetWorldTimerManager().SetTimer(GameTimeHandle, BakeryGameState, &ABakeryGameState::AddTime, TimeTickRate, true, TimeTickRate);

	USoundManager::GetInstance(GetWorld())->PlayBackgroundMusic();
}

void ABakeryGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// TODO: 데이터 로딩 / 해제 모두 한 곳에서 관리하기
	UAbnormalityManager::GetInstance(GetWorld())->RemoveFromRoot();
	USoundManager::GetInstance(GetWorld())->End();

	for (TActorIterator<AIngredientContainer>It(GetWorld()); It; ++It)
	{
		(*It)->End();
	}

	for (TActorIterator<ADishContainer>It(GetWorld()); It; ++It)
	{
		(*It)->End();
	}
}

void ABakeryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (BakeryGameState->GetElapsedTime() >= BakeryCloseTime)
	{
		if (BakeryGameState->GetBakeryState() == EBakeryState::Opened)
		{
			CloseBakery();
		}
	}
	else if (BakeryGameState->GetElapsedTime() >= BakeryOpenTime)
	{
		if (BakeryGameState->GetBakeryState() == EBakeryState::Closed)
		{
			OpenBakery();
		}
	}
}

void ABakeryGameMode::LoadUI()
{
	UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	UISubsystem->LoadAllUI();

	UUserWidget* HUDWidget = UISubsystem->SetUIVisibility(FName("BakeryHUD"), ESlateVisibility::SelfHitTestInvisible);
	BakeryHUDWidget = Cast<UBakeryHUDWidget>(HUDWidget);

	// TODO: 현재 시연용으로, 이상현상 종류와 발생 시간이 고정됨
	// 이상 현상 등록
	UAbnormalityManager* AbnormalityManager = UAbnormalityManager::GetInstance(GetWorld());
	FTimerHandle FixedHandle;
	GetWorld()->GetTimerManager().SetTimer(FixedHandle, AbnormalityManager, &UAbnormalityManager::RegisterFixedAbnormality, 0.01f, false);
}

void ABakeryGameMode::OpenBakery()
{
	if (!BakeryHUDWidget)
	{
		return;
	}

	BakeryHUDWidget->SetHUDState(true);

	BakeryGameState->SetTime(BakeryOpenTime);

	OnBakeryPreOpened.Broadcast();
	OnBakeryOpened.Broadcast();
}

void ABakeryGameMode::CloseBakery()
{
	BakeryHUDWidget->SetHUDState(false);

	BakeryGameState->SetTime(BakeryCloseTime);

	for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
	{
		(*It)->Leave();
	}

	UAbnormalityManager::GetInstance(GetWorld())->RegisterFixedAbnormality();

	OnBakeryPreClosed.Broadcast();
	OnBakeryClosed.Broadcast();
}

bool ABakeryGameMode::IsOpened()
{
	return BakeryHUDWidget && BakeryHUDWidget->GetHUDState();
}
