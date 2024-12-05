// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InkAbnormalityWidget.generated.h"

class UCanvasPanel;
class UImage;

/**
 * 
 */
UCLASS()
class BAKERY_API UInkAbnormalityWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Initailize(UTexture2D* InkImage, int InkNum, float InInkImageMinDistance = 300.f);

	void Shake();

	void FadeIn();
	void FadeOut();
	void Stop();

	UFUNCTION()
	void OnFadeInFinished();

	UFUNCTION()
	void OnFadeOutFinished();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* InkImageContainer;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InkFadeInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InkFadeOutAnim;

	TArray<UImage*> InkImages;
	bool bIsVisible = false;

	float InkImageMinDistance = 300.f;
};
