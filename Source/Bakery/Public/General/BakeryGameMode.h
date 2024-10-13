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
	virtual void Disappointed(APawn* Pawn) {}

	FBakeryOpendDelegate OnBakeryPreOpened;
	FBakeryOpendDelegate OnBakeryOpened;
	FBakeryClosedDelegate OnBakeryClosed;

protected:
	virtual void BeginPlay() override;

private:
	ABakeryGameState* BakeryGameState;

	UBakeryHUDWidget* BakeryHUDWidget;
	ABakeryPlayerController* PlayerController;
	TArray<ACustomerSpawner*> CustomerSpawners;

	UPROPERTY(EditAnywhere, Category="GameSetting")
	float OperatingTime = 30.f;
	float CurrentOperatingTime = 0.f;
};
