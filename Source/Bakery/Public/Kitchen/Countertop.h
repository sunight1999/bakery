// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/Interactables/GrabInteractableActor.h"
#include "Countertop.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCookingAnimStartedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCookingAnimStoppedDelegate);

enum class ECookingTool : uint8;
enum class EQuickSelectMenu : uint8;

class AIngredient;
class UIngredientData;
class URecipeSubsystem;
class UNiagaraComponent;
class UWidgetComponent;
class UProgressWidget;
class ACookingStateIndicator;
class UGrabberComponent;
class ADessertSceneCapturer;

/**
 * 재료를 올려두거나 요리할 수 있는 조리대
 */
UCLASS()
class BAKERY_API ACountertop : public AGrabInteractableActor
{
	GENERATED_BODY()

public:
	ACountertop();
	void Tick(float DeltaTime) override;

	void ResetCooking(bool bFullyReset = false);

	void HandleCook();
	void HandleQuickMenuClosed();

	virtual void OnEnterInteract(const FInteractionInfo& InteractionInfo) override;
	virtual void OnInteract(float DeltaTime) override;
	virtual void OnExitInteract() override;

	virtual void OnEnterGrab(const FInteractionInfo& InteractionInfo) override;
	virtual void OnGrab(float DeltaTime) override {}
	virtual void OnExitGrab() override {}

	/*
	 * 요리 관련 함수
	 */
	void BeginCook(ECookingTool CookingTool);
	void Cook();
	void EndCook();

	void PlayCookingAnimation();
	void StopCookingAnimation();

	FORCEINLINE float CalculateAutoShorten() const { return AutoCookingTime / HandCookingTime * AutoCookingReduceMultiplier; }
	FORCEINLINE bool IsCookingDone() const
	{
		if (bIsAutomatic)
		{
			return CurrentAutoCookingTime >= AutoCookingTime;
		}
		else
		{
			return CurrentHandCookingTime >= HandCookingTime;
		}
	}

	UPROPERTY(BlueprintAssignable)
	FCookingAnimStartedDelegate OnCookingAnimStarted;

	UPROPERTY(BlueprintAssignable)
	FCookingAnimStoppedDelegate OnCookingAnimStopped;

protected:
	virtual void BeginPlay() override;

private:
	/*
	 * 퀵 메뉴 관련
	 */
	UPROPERTY(EditAnywhere, Category = "Countertop")
	TArray<ECookingTool> ToolsQuickMenuValues;

	UGrabberComponent* PendingGrabber = nullptr;

	/*
	 * 메쉬 관련
	 */
	UPROPERTY(VisibleAnywhere, Category="Countertop")
	UStaticMeshComponent* OptionalToolMesh;

	UPROPERTY(VisibleAnywhere, Category="Countertop")
	UStaticMeshComponent* TopPlateMesh;

	UPROPERTY(VisibleAnywhere, Category="Countertop")
	UStaticMeshComponent* BottomSupportMesh;

	UPROPERTY(VisibleAnywhere, Category="Countertop")
	USceneComponent* KeepPoint;

	UPROPERTY(VisibleAnywhere, Category = "Countertop")
	USceneComponent* CookingStateIndicatorPoint;

	/*
	 * 요리 관련
	 */
	UPROPERTY(VisibleAnywhere, Category = "Countertop|Cooking")
	UWidgetComponent* CookingProgressWidget;

	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	UNiagaraComponent* PrimaryCookingEffect;

	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	TSet<ECookingTool> AvailableCookingTools;

	// true인 경우 조리가 완료됐지만 bIsPendingCooking = true인 재료에 대해 완료 처리를 진행할 수 있음
	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	bool bIsPendingCookingCompletable = false;

	// true인 경우 조리대에서 요리 가능한 재료라면 자동으로 요리 진행, 일정 시간 후 완료
	// false인 경우 플레이어가 상호작용 키를 여러 번 눌러 요리를 진행해야 함
	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	bool bIsAutomatic = false;

	// 자동으로 요리를 완성하는데 걸리는 시간
	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	float AutoCookingTime = 5.f;

	// 자동 요리 시간 단축 기본값인 (AutoCookingTime / HandCookingTime)에 곱할 배수
	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	float AutoCookingReduceMultiplier = .5f;

	// 요리를 완성하기 위해 필요한 상호작용 횟수
	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	int HandCookingTime = 10;

	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	TSubclassOf<ACookingStateIndicator> CookingStateIndicatorClass;

	UPROPERTY(EditAnywhere, Category = "Countertop|Cooking")
	FRotator CookingStateFixedRotation = FRotator(0.f, 180.f, 0.f);

	UProgressWidget* CookingProgress;
	ACookingStateIndicator* CookingStateIndicator;

	/*
	 * 현재 보관 중인 재료 또는 요리 관련
	 */
	AActor* CurrentKeptObject;
	AIngredient* CurrentKeptIngredient;

	UAudioComponent* CurrentAudio;

	ECookingTool CurrentCookingTool;
	float CurrentAutoCookingTime = 0.f;
	int CurrentHandCookingTime = 0;

	bool bIsCooking = false;

	/*
	 * 캐싱
	 */ 
	const URecipeSubsystem* RecipeSubsystem;
};
