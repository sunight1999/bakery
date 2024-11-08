#pragma once
#include "AbnormalitiesReport/AbnormalityManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Image.h"
#include "AbnormalityEvent.generated.h"


/**
 * 상황보고 이벤트 관리 시스템
 */
class UFadeImageWidget;

enum EEventType
{
	EVT_AbnormalityTriggerEvent,//트리거 발동 이상현상
	EVT_AbnormalityStatEvent,	//캐릭터 스탯 조절 이상현상
	EVT_AbnormalitySpawnEvent,	//액터 소환 이상현상
};

UCLASS()
class BAKERY_API AAbnormalityEvent : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomPosMin = 0.1;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomPosMax = 0.9;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomSizeMin	= 0.5;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomSizeMax = 2.5;
	int RandNum;
	UPROPERTY(EditAnywhere, Category = "Fade")
	TArray<TSubclassOf<UUserWidget>> FadeWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Fade")
	int MaxFadeArrayNum = 5;
	UPROPERTY(EditAnywhere, Category = "Fade")
	class UFadeImageWidget* FadeWidgetInstance;
	UPROPERTY(EditAnywhere, Category = "Fade")
	TArray<UFadeImageWidget*> FadeWidgetInstanceArray;
	bool EventPlay;
public:
	void PlayEvent(int num);
	void TestPlayEvent(); // 이벤트 실행
public:
	void TriggerBlurEffect(); // 시야 방해;
	void TriggerBlurEffectInitialize(); // 초기화
};
 