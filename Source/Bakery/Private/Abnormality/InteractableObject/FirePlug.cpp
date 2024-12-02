#include "Abnormality/InteractableObject/FirePlug.h"
#include "Components/WidgetComponent.h"
#include "Widgets/Progress/ProgressWidget.h"

void AFirePlug::BeginPlay()
{
	if (ProgressComponent) {
		ProgressComponent->GetUserWidgetObject();
		ProgressWidget = Cast<UProgressWidget>(ProgressComponent);
	}
}

void AFirePlug::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
}
void AFirePlug::OnInteract(float deltatime)
{
	if (CurrentGauge < MaxGauge && !EventFlag) {
		CurrentGauge += GuageSpeed * deltatime;
		//ProgressWidget->SetPercentage(CurrentGauge);
		UE_LOG(LogTemp, Display, TEXT("Test : %f"),CurrentGauge);
	}
	if (CurrentGauge == MaxGauge) {
		EventFlag = true;
		// 이벤트 실행 함수 추가
	}
}
void AFirePlug::OnExitInteract()
{
	if (CurrentGauge < MaxGauge) {
		CurrentGauge = 0;
		//ProgressWidget->SetPercentage(0);
	}
}
void AFirePlug::SetEventFlag(bool Flag)
{
	EventFlag = Flag;
}
