#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbnormalitiesReport/AbnormalitiesReportManager.h"
#include "AbnormalitiesReportWidget.generated.h"
/**
 * AbnormalitiesReport클래스의 위젯을 담당하는 UAbnormalitiesReportWidget
 */
class AAbnormalitiesReportManager;
UCLASS()
class BAKERY_API UAbnormalitiesReportWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget), VisibleAnywhere, Category = "UI")
	class UTextBlock* HeadTextBlock;
	UPROPERTY(meta = (BindWidget), VisibleAnywhere, Category = "UI")
	class UTextBlock* TextBlock;
	UPROPERTY(meta = (BindWidget), VisibleAnywhere, Category = "UI")
	class UButton* ExitButton;
	int32 ChoiceImage;
	UFUNCTION()
	void OnOverlay();
	UFUNCTION()
	void OffOverlay();
	UFUNCTION()
	void ChangeReportHeadText(FString Text);
	UFUNCTION()
	void ChangeReportText(FString Text);
	bool Initialize() override;
};