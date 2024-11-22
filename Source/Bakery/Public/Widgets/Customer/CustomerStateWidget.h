// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomerStateWidget.generated.h"

#define WAITING_ORDER_ICON_BASE_PATH FString("/Game/Textures/Icon/Order/")
#define WAITING_ORDER_ICON_ORDERING_PATH WAITING_ORDER_ICON_BASE_PATH + FString("Bell")

class UCanvasPanel;
class UImage;
class UProgressBar;

enum class EWaitingState : uint8
{
	None,
	Nice,
	NotBad,
	Bad
};

/**
 * 
 */
UCLASS()
class BAKERY_API UCustomerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/*
	 * 대기시간 관련 함수
	 */
	UFUNCTION(BlueprintCallable)
	float GetWaitingTimeBarPercentage() const { return WaitingTimeBarPercentage; }

	void ResetWaitingTimeBar();
	void SetWaitingIconImage(FString ImagePath);
	void SetWaitingTimeBarColor(const FSlateColor& NewColor);
	void SetWaitingTimeBarVisibility(bool bVisible);
	void SetWaitingTimeBarPercentage(float InPercentage);

	/*
	 * 공포 수치 관련 함수
	 */
	UFUNCTION(BlueprintCallable)
	float GetFearBarPercentage() const { return FearBarPercentage; }

	void SetFearBarVisibility(bool bVisible);
	void SetFearBarPercentage(float InPercentage);

private:
	/*
	 * 대기시간 관련
	 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* WaitingTimeBarCanvas;

	UPROPERTY(meta = (BindWidget))
	UImage* WaitingIconImage;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* WaitingTimeBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* WaitingTimeBarAppearAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* WaitingTimeBarShakeAnim;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float WaitingNotBadStateThreshold = .6f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float WaitingBadStateThreshold = .3f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	FColor WaitingNiceStateColor;

	UPROPERTY(EditAnywhere, Category = "Customer");
	FColor WaitingNotBadStateColor;

	UPROPERTY(EditAnywhere, Category = "Customer");
	FColor WaitingBadStateColor;

	EWaitingState WaitingState = EWaitingState::None;
	bool bIsWaitingTimeBarVisible = false;
	float WaitingTimeBarPercentage = 1.f;

	/*
	 * 공포 수치 관련
	 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* FearBarCanvas;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* FearBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FearBarAppearAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FearBarShakeAnim;

	bool bIsFearBarVisible = false;
	float FearBarPercentage = 0.f;
};
