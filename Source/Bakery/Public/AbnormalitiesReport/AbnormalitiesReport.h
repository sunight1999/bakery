
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

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TArray<FSituationData&> SituationArray;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=())
	//FDataTableRowHandle Situation;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UUserWidget* OverlayWidgetInstance;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, Category = "UI")
	class UTextBlock* TextBlock;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, Category = "UI")
	class UButton* ExitButton;

	/*
	 * UI 관련 세팅 변수
	 */

	UPROPERTY(EditAnywhere, Category = "UI_Settings")
	bool IsCreate;

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

	UPROPERTY(EditAnywhere, Category = "UI_Settings_Random")
	bool IsProbabilityStart;

	UPROPERTY(EditAnywhere, Category = "UI_Settings_Random")
	float Probability;

	UPROPERTY(EditAnywhere, Category = "UI_Settings_Random")
	float ProbabilityStat;

	UPROPERTY()
	int Random;
public:
	void SetFuncFlag();
	void CreateOverlayWidget();
	void CreateRandomAnswer();
	void SituationCoolDown();
	UFUNCTION()
	void OnOverlay();
	UFUNCTION()
	void OffOverlay();
	UFUNCTION()
	void TestPrint();
	void ChangeText(FString Text);
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
