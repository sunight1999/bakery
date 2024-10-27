#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Engine/GameViewportClient.h"
#include "Animation/AnimInstance.h"
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

private:
	void OnFadeImage();
	void OffFadeImage();
	void OnImage();
	void OffImage();
	bool Initialize();
};
