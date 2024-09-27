// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalitiesReport.generated.h"

UCLASS()
class BAKERY_API AAbnormalitiesReport : public AActor
{
	GENERATED_BODY()
	
public:	
	// 위젯 클래스를 저장할 UPROPERTY
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> WidgetClass;

	// 실제 UI 위젯을 참조할 변수
	UPROPERTY()
	UUserWidget* CurrentWidget;

	// UMG의 TextBlock을 바인딩 (블루프린트에서 텍스트를 할당)
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MyTextBlock;
	
	AAbnormalitiesReport();
	UPROPERTY(EditAnywhere)
	uint32 Situation = 5;

	UPROPERTY(EditAnywhere)
	uint32 SituationChoice=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
	float CooldownTime = 5;

	UFUNCTION()
	void OnCooldown(); // 타이머 호출 함수

	UFUNCTION()
	void ShowCooldownMessage(); // 타이머에 따른 메시지 표기 함수
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FTimerHandle SituationCountHandle;
	void TimeCount(float Time);

	void TestLog(int Choice1);
	void Choice();
};
