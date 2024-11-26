// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomerStateWidget.generated.h"

#define WAITING_ORDER_ICON_BASE_PATH FString("/Game/Textures/Icon/Order/")
#define WAITING_ORDER_ICON_ORDERING_PATH WAITING_ORDER_ICON_BASE_PATH + FString("Bell")

UENUM(BlueprintType)
enum class EWaitingState : uint8
{
	Nice,
	NotBad,
	Bad
};

class UCanvasPanel;
class UImage;
class UProgressBar;

DECLARE_MULTICAST_DELEGATE_OneParam(FWaitingStateChanged, EWaitingState);

/**
 * 
 */
UCLASS()
class BAKERY_API UCustomerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProgressBarColor(UProgressBar* ProgressBar, const FSlateColor& NewColor);

	/*
	 * 대기시간 관련 함수
	 */
	UFUNCTION(BlueprintCallable)
	float GetWaitingTimeBarPercentage() const { return WaitingTimeBarPercentage; }

	void ResetWaitingTimeBar();
	void SetWaitingIconImage(FString ImagePath);
	void SetWaitingTimeBarVisibility(bool bVisible);
	void SetWaitingTimeBarPercentage(float InPercentage);

	FWaitingStateChanged OnWaitingStateChanged;

	/*
	 * 공포 수치 관련 함수
	 */
	UFUNCTION(BlueprintCallable)
	float GetFearBarPercentage() const { return FearBarPercentage; }

	void ResetFearBar();
	void SetFearBarVisibility(bool bVisible);
	void SetFearBarPercentage(float InPercentage);

	FWaitingStateChanged OnFearStateChanged;

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

	EWaitingState WaitingState = EWaitingState::Nice;
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

	UPROPERTY(EditAnywhere, Category = "Customer");
	float FearNotBadStateThreshold = .3f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	float FearBadStateThreshold = .6f;

	UPROPERTY(EditAnywhere, Category = "Customer");
	FColor FearNiceStateColor;

	UPROPERTY(EditAnywhere, Category = "Customer");
	FColor FearNotBadStateColor;

	UPROPERTY(EditAnywhere, Category = "Customer");
	FColor FearBadStateColor;

	EWaitingState FearState = EWaitingState::Nice;
	bool bIsFearBarVisible = false;
	float FearBarPercentage = 0.f;
};
