// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Subsystems/UISubsystem.h"

#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "General/BakeryGameMode.h"
#include "General/BakeryGameState.h"
#include "Widgets/Menu/QuickSelectMenuWidget.h"

APlayerPawn::APlayerPawn()
{
	Interactor = CreateDefaultSubobject<UInteractorComponent>(TEXT("Interactor"));
	Interactor->SetupAttachment(RootComponent);

	StateWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StateWidget"));
	StateWidget->SetupAttachment(RootComponent);
	StateWidget->SetVisibility(false);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	Interactor->GetGrabber()->OnGrab.AddUObject(this, &APlayerPawn::HandsUp);
	Interactor->GetGrabber()->OnRelease.AddUObject(this, &APlayerPawn::HandsDown);

	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	FTimerHandle UILoadHandle;
	GetWorld()->GetTimerManager().SetTimer(UILoadHandle, BakeryGameMode, &ABakeryGameMode::LoadUI, 0.1f, false);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	verify(MoveAction);
	verify(InteractAction);
	verify(GrabAction);
	
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, Interactor, &UInteractorComponent::BeginInteraction);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Completed, Interactor, &UInteractorComponent::EndInteraction);
		EnhancedInput->BindAction(GrabAction, ETriggerEvent::Started, Interactor, &UInteractorComponent::BeginGrab);
		EnhancedInput->BindAction(GrabAction, ETriggerEvent::Completed, Interactor, &UInteractorComponent::EndGrab);
		EnhancedInput->BindAction(StartBakeryAction, ETriggerEvent::Started, this, &APlayerPawn::StartBakery);
		EnhancedInput->BindAction(TabAction, ETriggerEvent::Started, this, &APlayerPawn::OpenAbnormalForecastMenu);
	}
}

void APlayerPawn::SetPlayerState(EPlayerState InState)
{
	StateWidget->SetVisibility(true);

	switch (InState)
	{
	case EPlayerState::Normal:
		StateWidget->SetVisibility(false);
		break;

	case EPlayerState::SpeedDown:
		StateWidget->SetWidgetClass(SpeedDownStateWidgetClass);
		break;

	default:
		break;
	}
}

void APlayerPawn::SetUIOpened(bool bIsOpened)
{
	bIsUIOpened = bIsOpened;

	// TODO: 추후 동적 오브젝트 생성(초기화)/파괴 한 곳으로 정리 후, 생성 완료 Delegate에 QuickMenu 초기화 등록
	if (!QuickMenu)
	{
		UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
		UUserWidget* UserWidget = UISubsystem->GetUIObject(FName("QuickSelectMenu"));
		QuickMenu = Cast<UQuickSelectMenuWidget>(UserWidget);
	}
}

bool APlayerPawn::IsGrabbing()
{
	return Interactor->GetGrabber()->IsGrabbing();
}

void APlayerPawn::EnsureHandsDownTimerDone()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(HandsDownTimer))
	{
		TimerManager.ClearTimer(HandsDownTimer);
		HandsDown();
	}
}

void APlayerPawn::HandsUp()
{
	EnsureHandsDownTimerDone();

	if (PlayerMontage)
	{
		PlayAnimMontage(PlayerMontage, 2.f, FName("Grab"));
	}
}

void APlayerPawn::HandsCook()
{
	EnsureHandsDownTimerDone();

	if (PlayerMontage)
	{
		PlayAnimMontage(PlayerMontage, 2.f, FName("Cook"));
	}
	
	GetWorld()->GetTimerManager().SetTimer(HandsDownTimer, this, &APlayerPawn::HandsDown, 1.2f);
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// 퀵 메뉴가 열렸을 경우 처리
	if (bIsUIOpened && QuickMenu->IsShowing())
	{
		int Direction = 0;

		bool bIsXZero = FMath::IsNearlyEqual(MovementValue.X, 0);
		bool bIsYZero = FMath::IsNearlyEqual(MovementValue.Y, 0);

		if (bIsXZero == bIsYZero)
		{
			return;
		}

		if (bIsXZero)
		{
			Direction = FMath::IsNearlyEqual(MovementValue.Y, 1) ? 0 : 2;
		}
		else
		{
			Direction = FMath::IsNearlyEqual(MovementValue.X, 1) ? 1 : 3;
		}

		QuickMenu->SetFocus(Direction);
		return;
	}

	AddMovementInput(FVector::ForwardVector, MovementValue.Y);
	AddMovementInput(FVector::RightVector, MovementValue.X);

	EnsureHandsDownTimerDone();
}

/*
 * UI 관련 함수
 */
void APlayerPawn::OpenMenu(FName Menu)
{
	UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	UISubsystem->SetUIVisibility(Menu, ESlateVisibility::SelfHitTestInvisible);

	SetUIOpened(true);
}

void APlayerPawn::OpenAbnormalForecastMenu()
{
	OpenMenu(FName("AbnormalForecast"));
}

void APlayerPawn::StartBakery()
{
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	if (BakeryGameState->GetBakeryState() == EBakeryState::Opened)
	{
		return;
	}

	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	BakeryGameMode->OpenBakery();
}
