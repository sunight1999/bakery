// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "Highlight.h"
#include "InteractableActor.generated.h"

enum class EQuickSelectMenu : uint8;
class UBoxComponent;
class UQuickSelectMenuWidget;

UCLASS()
class BAKERY_API AInteractableActor : public AActor, public IInteract, public IHighlight
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExitInteract() override;


	UFUNCTION(BlueprintCallable)
	virtual void OnEnterHighlight() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExitHighlight() override;

protected:
	virtual void BeginPlay() override;

	/*
	 * 퀵 메뉴 관련
	 */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	EQuickSelectMenu QuickSelectMenuType;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float QuickSelectMenuShowDelay = .3f;

	bool bQuickSelectMenuOpened = false;
	UQuickSelectMenuWidget* QuickSelectMenu = nullptr;
	int CurrentQuickSelectIndex = 0;
	float CurrentInteractPressedTime = 0.f;

	/*
	 * 그 외
	 */
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	UMaterialInterface* HighlightOverlayMaterial;
};
