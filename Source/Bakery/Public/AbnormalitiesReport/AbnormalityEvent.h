#pragma once
#include "AbnormalitiesReport/AbnormalityManager.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Components/Image.h"
#include "AbnormalityEvent.generated.h"


/**
 * 상황보고 이벤트 관리 시스템
 */

enum EEventType
{
	EVT_AbnormalityTriggerEvent,//트리거 발동 이상현상
	EVT_AbnormalityStatEvent,	//캐릭터 스탯 조절 이상현상
	EVT_AbnormalitySpawnEvent,	//액터 소환 이상현상
};

UCLASS()
class BAKERY_API UAbnormalityEvent : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UAbnormalityEvent();

	int RandNum;
	UPROPERTY(EditAnywhere, Category = "Fade")
	TSubclassOf<UUserWidget> FadeWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Fade")
	int MaxFadeArrayNum = 3;
	UPROPERTY(EditAnywhere, Category = "Fade")
	class UFadeImageWidget* FadeWidgetInstance;
	UPROPERTY(EditAnywhere, Category = "Fade")
	TArray<UFadeImageWidget*> FadeWidgetInstanceArray;

public:
	void PlayEvent(int num);
	void TestPlayEvent(); // 이벤트 실행
public:
	void TriggerBlurEffect(); // 시야 방해;
	void TriggerBlurEffectInitialize(); // 초기화
	void RandomlyPlaceImage(UStaticMeshComponent* Image, FVector2D MinBounds, FVector2D MaxBounds);
};
 