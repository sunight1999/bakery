// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BakeryGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FBakeryOpendDelegate);
DECLARE_MULTICAST_DELEGATE(FBakeryClosedDelegate);

class ABakeryGameState;
class UUISubsystem;
class UBakeryHUDWidget;
class ABakeryPlayerController;
class ACustomerSpawner;

UCLASS(minimalapi)
class ABakeryGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABakeryGameMode();
	virtual void Tick(float DeltaSeconds) override;

	virtual void OpenBakery();
	virtual void CloseBakery();

	bool IsOpened();
	virtual void Disappointed(APawn* Pawn) {}

	float GetBakeryOpenTime() const { return BakeryOpenTime; }
	float GetBakeryCloseTime() const { return BakeryCloseTime; }

	FBakeryOpendDelegate OnBakeryPreOpened;
	FBakeryOpendDelegate OnBakeryOpened;
	FBakeryClosedDelegate OnBakeryClosed;

protected:
	virtual void BeginPlay() override;

private:
	// 게임 시간 배속
	UPROPERTY(EditAnywhere, Category = "GameSetting")
	float GameTimeMultiplier = 2;

	// 게임 시작 시 설정 시간 (분)
	UPROPERTY(EditAnywhere, Category = "GameSetting")
	int GameStartTime = 7 * 60;

	// 영업 시작 시간 (분)
	UPROPERTY(EditAnywhere, Category = "GameSetting")
	int BakeryOpenTime = 9 * 60;

	// 영업 종료 시간 (분)
	UPROPERTY(EditAnywhere, Category = "GameSetting")
	int BakeryCloseTime = 18 * 60;

	ABakeryGameState* BakeryGameState;

	UBakeryHUDWidget* BakeryHUDWidget;
	ABakeryPlayerController* PlayerController;
	TArray<ACustomerSpawner*> CustomerSpawners;

	FTimerHandle GameTimeHandle;
};
