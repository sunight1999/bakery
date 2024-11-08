#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Engine/GameViewportClient.h"
#include "Animation/AnimInstance.h"
#include <Components/CanvasPanel.h>
#include "FadeImageWidget.generated.h"


/**
 * 얼룩 이벤트 이미지
 */
UCLASS()
class BAKERY_API UFadeImageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	int ImageNum;
	UPROPERTY(meta = (BindWidget), VisibleAnywhere, Category = "Image")
	UImage* BlurImage;
	UPROPERTY(meta = (BindWidget), EditAnywhere, Category = "Canvas")
	UCanvasPanel* BlurImageCanvas;
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, Transient, Category = "Animation")
	UWidgetAnimation* OnFadeImageAnim;
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, Transient, Category = "Animation")
	UWidgetAnimation* OffFadeImageAnim;
	
	int32 ChoiceImage;
	bool IsFade;
public:
	void OnFadeImage();
	void OffFadeImage();
	void OnImage();
	void OffImage();
	void VisibleImage();
	void HiddenImage();
	FVector2D GetrandomPercentage(float MinPercentage, float MaxPercentage);
	void SetRandomImagePosition(float RandomPosMin, float RandomPosMax);
	void SetRandomImageScale(float RandomSizeMin, float RandomSizeMax);
	void RandomChoiceImage(int32 Min, int32 Max);
private:
	
	bool Initialize();
	
};
