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
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, Transient, Category = "Animation")
	UWidgetAnimation* OnFadeImageAnim;
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, Transient, Category = "Animation")
	UWidgetAnimation* OffFadeImageAnim;
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, Transient, Category = "Animation")
	UWidgetAnimation* LoopFadeImageAnim;

	float RandomPosMin;
	float RandomPosMax;
	float RandomSizeMin;
	float RandomSizeMax;
	FBox2D Bound;
	FTimerHandle FadeLoopTimer;

	float LoopInterval = 5.0f;
	int32 ChoiceImage;
	bool IsLoop;
public:
	void OnFadeImage();
	void OffFadeImage();
	void LoopFadeImage();
	void OnImage();
	void OffImage();
	void VisibleImage();
	void HiddenImage();
	FTimerHandle& GetLoopTimer();
	FVector2D GetrandomPercentage(float MinPercentage, float MaxPercentage);
	void SetRandomImagePosition();
	void SetRandomImageScale();
	void SetImagePos(FVector2D Pos);
	void SetWidgetBounds();
	FBox2D GetWidgetBounds();
	FVector2D GetWidgetPos();
	double GetLoopInterval();
	void SetSetting();
	void RandomChoiceImage(int32 Min, int32 Max);

	void SetRandomPosMinMax(float PosMin, float PosMax);
	void SetRandomSizeMinMax(float SizeMin, float SizeMax);
	
private:
	bool Initialize();
	
};
