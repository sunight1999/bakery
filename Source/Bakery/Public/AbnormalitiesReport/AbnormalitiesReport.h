
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalitiesReport/SituationDatabase.h"
#include "AbnormalitiesReport.generated.h"

UCLASS()
class BAKERY_API AAbnormalitiesReport : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbnormalitiesReport();

	/*
	 * UI 기본 변수
	 */
	UPROPERTY(VisibleAnywhere, Category = "Data")
	UDataTable* DataTable;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> OverlayWidgetClass;
	//class UAbnormalitiesReportWidget OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UAbnormalitiesReportWidget* OverlayWidgetInstance;
	/*
 * UI 관련 세팅 변수
 */
	UPROPERTY(EditAnywhere, Category = "UI_Settings")
	bool IsCreate;
	UPROPERTY(EditAnywhere, Category = "UI_Settings")
	bool IsView;
	UPROPERTY(EditAnywhere, Category = "UI_Settings")
	float CooldownTime;
	UPROPERTY(VisibleAnywhere, Category = "UI_Settings")
	struct FTimerHandle CooldownTimerHandle;
	UPROPERTY(EditAnywhere, Category = "UI_Settings")
	int32 SituationNum;
	/*
	 * UI 확률 관련 세팅 변수
	 */
	UPROPERTY(EditAnywhere, Category = "UI_Settings_Random")
	bool IsProbability;

	void SetFuncFlag();
	void CreateOverlayWidget();
	void CreateRandomAnswer();
	void SituationCoolDown();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};