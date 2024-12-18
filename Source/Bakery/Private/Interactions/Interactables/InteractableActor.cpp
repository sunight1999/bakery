// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/Interactables/InteractableActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Interactions/InteractionDefines.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/Menu/QuickSelectMenuWidget.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetCollisionProfileName(TEXT("Interactable"));
	RootComponent = InteractionBox;

	HighlightOverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/ETC/Overlays/MI_InteractableOverlay"));

	QuickSelectMenuType = EQuickSelectMenu::None;
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableActor::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	if (QuickSelectMenuType == EQuickSelectMenu::None)
	{
		return;
	}

	if (!QuickSelectMenu)
	{
		UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
		UUserWidget* UserWidget = UISubsystem->GetUIObject(FName("QuickSelectMenu"));
		QuickSelectMenu = Cast<UQuickSelectMenuWidget>(UserWidget);
	}

	// 액터 위치로 퀵 메뉴 UI 위치 설정
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenLocation;
	if (PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenLocation))
	{
		QuickSelectMenu->SetPositionInViewport(ScreenLocation / GEngine->GameViewport->GetDPIScale());
	}
	
	bQuickSelectMenuOpened = false;
	CurrentInteractPressedTime = 0.f;
}

void AInteractableActor::OnInteract(float DeltaTime)
{
	if (QuickSelectMenuType == EQuickSelectMenu::None)
	{
		return;
	}

	if (!QuickSelectMenu->IsShowing())
	{
		CurrentInteractPressedTime += DeltaTime;

		if (CurrentInteractPressedTime >= QuickSelectMenuShowDelay)
		{
			bQuickSelectMenuOpened = true;
			QuickSelectMenu->SetMenu(QuickSelectMenuType);
			QuickSelectMenu->Show(CurrentQuickSelectIndex);
		}
	}
}

void AInteractableActor::OnExitInteract()
{
	if (QuickSelectMenuType == EQuickSelectMenu::None)
	{
		return;
	}

	if (QuickSelectMenu->IsShowing())
	{
		CurrentQuickSelectIndex = QuickSelectMenu->Hide();
	}
}

void AInteractableActor::OnEnterHighlight()
{
	if (!HighlightOverlayMaterial)
	{
		return;
	}

	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);

	for (auto MeshComponent : MeshComponents)
	{
		MeshComponent->SetOverlayMaterial(HighlightOverlayMaterial);
	}
}

void AInteractableActor::OnExitHighlight()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);

	for (auto MeshComponent : MeshComponents)
	{
		MeshComponent->SetOverlayMaterial(nullptr);
	}
}
