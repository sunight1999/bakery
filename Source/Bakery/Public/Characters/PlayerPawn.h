// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class UInteractorComponent;
class UWidgetComponent;
class UQuickSelectMenuWidget;

enum class EQuickSelectMenu : uint8;
enum class EPlayerState : uint8
{
	Normal,
	SpeedDown
};

UCLASS()
class BAKERY_API APlayerPawn : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	EPlayerState GetPlayerState() const { return State; }
	void SetPlayerState(EPlayerState InState);

	bool IsUIOpened() const { return bIsUIOpened; }
	void SetUIOpened(bool bIsOpened);
	
	bool IsGrabbing();

	void EnsureHandsDownTimerDone();
	void HandsUp();
	void HandsCook();
	void HandsDown() { if (PlayerMontage) StopAnimMontage(PlayerMontage); }

	void OpenMenu(FName Menu);
	void OpenAbnormalForecastMenu();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void StartBakery();

private:
	/*
	 * Input Actions
	 */
	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* StartBakeryAction;

	UPROPERTY(EditAnywhere, Category = "Player|Interaction")
	UInputAction* TabAction;

	/*
	 * 상호작용 및 이동 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Player|Interaction")
	UInteractorComponent* Interactor;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	float TurnSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Player|Movement")
	float MeshTurnOffset = -90.f;

	/*
	 * 애니메이션 관련
	 */
	UPROPERTY(EditAnywhere, Category = "Player|Animation")
	UAnimMontage* PlayerMontage;

	FTimerHandle HandsDownTimer;

	/*
	 * 상태 UI 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Player|Animation")
	UWidgetComponent* StateWidget;

	UPROPERTY(EditAnywhere, Category = "Player|Animation")
	TSubclassOf<UUserWidget> SpeedDownStateWidgetClass;

	UQuickSelectMenuWidget* QuickMenu;

	bool bIsUIOpened = false;
	EPlayerState State = EPlayerState::Normal;
};
