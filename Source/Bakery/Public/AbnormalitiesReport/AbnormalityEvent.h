#pragma once
#include "AbnormalitiesReport/AbnormalityManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Image.h"
#include "General/BakeryGameMode.h"
#include "AbnormalityEvent.generated.h"


/**
 * 상황보고 이벤트 관리 시스템
 */
class UFadeImageWidget;
class BakeryGameMode;
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
	AAbnormalityEvent();
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomPosMin = -0.4;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomPosMax = 0.4;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomSizeMin	= 1;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomSizeMax = 2.5;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float RandomPosMinDistance = 1;
	UPROPERTY(EditAnywhere, Category = "Fade")
	float LoopInterval = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Fade")
	TArray<TSubclassOf<UUserWidget>> FadeWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Fade")
	int MaxFadeArrayNum = 5;
	UPROPERTY(EditAnywhere, Category = "Fade")
	class UFadeImageWidget* FadeWidgetInstance;
	UPROPERTY(EditAnywhere, Category = "Fade")
	TArray<UFadeImageWidget*> FadeWidgetInstanceArray;
	UPROPERTY(EditAnywhere, Category = "bool")
	bool TestBool;
	int RandNum;
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void PlayEvent(int num);
	void ChoiceEvent();
	void TestPlayEvent(); // 이벤트 실행
	void SetEventNum(int num);
public:
	void TriggerBlurEffectInitialize(); // 초기화
	void TriggerBlurEffectStop(); // 시야 방해 중지
	void FadeLoop();
	void FadeLoopStop();
private:
	ABakeryGameMode* BakeryGameMode;
	FTimerHandle FadeLoopTimer;
	FTimerHandle RoundTimerHandle;
	bool EventPlay;
	bool FirstBlurEffect = true;
	int EventNum;
private:
	void SetPlaceWidget();
};
 