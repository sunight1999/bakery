// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BakeryGameMode.generated.h"

class ABakeryGameState;
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

protected:
	virtual void BeginPlay() override;

private:
	ABakeryGameState* BakeryGameState;

	ABakeryPlayerController* PlayerController;
	TArray<ACustomerSpawner*> CustomerSpawners;

	UPROPERTY(EditAnywhere, Category="GameSetting")
	float OperatingTime = 30.f;
	float CurrentOperatingTime = 0.f;
};
