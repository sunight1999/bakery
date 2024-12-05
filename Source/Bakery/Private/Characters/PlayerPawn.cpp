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

	QuickMenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuickMenuWidget"));
	QuickMenuWidget->SetupAttachment(RootComponent);
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

	QuickMenu = Cast<UQuickSelectMenuWidget>(QuickMenuWidget->GetWidget());
	verify(QuickMenu);

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

	// TODO: state 비트 플래그로 변경
	// 하나의 Widget에서 onoff로 state 표현할 수 있게 변경
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
}

UQuickSelectMenuWidget* APlayerPawn::SetQuickMenu(EQuickSelectMenu Menu)
{
	QuickMenu->SetMenu(Menu);

	return QuickMenu;
}

void APlayerPawn::ShowQuickMenu(int InitalizeIndex)
{
	SetUIOpened(true);
	QuickMenu->Show(InitalizeIndex);
}

int APlayerPawn::HideQuickMenu()
{
	SetUIOpened(false);
	return QuickMenu->Hide();
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

	// TODO: 코드 리팩토링 필요
	if (bIsUIOpened)
	{
		if (QuickMenu->IsShowing())
		{
			int Direction = 0;

			if (FMath::IsNearlyEqual(MovementValue.X, 0))
			{
				if (FMath::IsNearlyEqual(MovementValue.Y, 1))
				{
					Direction = 0;
				}
				else if (FMath::IsNearlyEqual(MovementValue.Y, -1))
				{
					Direction = 2;
				}
				else
				{
					return;
				}
			}
			else if (FMath::IsNearlyEqual(MovementValue.Y, 0))
			{
				if (FMath::IsNearlyEqual(MovementValue.X, 1))
				{
					Direction = 1;
				}
				else if (FMath::IsNearlyEqual(MovementValue.X, -1))
				{
					Direction = 3;
				}
				else
				{
					return;
				}
			}

			QuickMenu->SetFocus(Direction);
		}
		
		return;
	}

	AddMovementInput(FVector::ForwardVector, MovementValue.Y);
	AddMovementInput(FVector::RightVector, MovementValue.X);

	EnsureHandsDownTimerDone();
}

void APlayerPawn::OpenAbnormalForecastMenu()
{
	SetUIOpened(true);
	UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	UISubsystem->SetUIVisibility(FName("AbnormalForecast"), ESlateVisibility::SelfHitTestInvisible);
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
