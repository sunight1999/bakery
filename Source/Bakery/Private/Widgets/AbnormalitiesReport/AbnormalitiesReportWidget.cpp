#include "Widgets/AbnormalitiesReport/AbnormalitiesReportWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include <AbnormalitiesReport/AbnormalitiesReportManager.h>
#include "AbnormalitiesReport/\AbnormalityEvent.h"
#include "Widgets/AbnormalitiesReport/AbnormalitiesReportWidget.h"
void UAbnormalitiesReportWidget::OnOverlay()
{
    SetVisibility(ESlateVisibility::Visible);  // 인스턴스를 통해 함수 호출
}
void UAbnormalitiesReportWidget::OffOverlay()
{
    SetVisibility(ESlateVisibility::Hidden);  // 인스턴스를 통해 함수 호출
}
void UAbnormalitiesReportWidget::ChangeReportHeadText(FString Text)
{
    HeadTextBlock->SetText(FText::FromString(Text));
}
void UAbnormalitiesReportWidget::ChangeReportText(FString Text)
{
    TextBlock->SetText(FText::FromString(Text));
}

bool UAbnormalitiesReportWidget::Initialize()
{
    Super::Initialize();
    ExitButton->OnClicked.AddDynamic(this, &UAbnormalitiesReportWidget::OffOverlay);
    return true;
}//UTextBlock* UAbnormalitiesReportWidget::ChangeText(AAbnormalitiesReportManager* Report)
//{
//    UTextBlock* TextBlock = Cast<UTextBlock>(Report->OverlayWidgetInstance->GetWidgetFromName(TEXT("TestBlock")));
//    return TextBlock;
//}
//UButton* UAbnormalitiesReportWidget::ExitButtonBinding(AAbnormalitiesReportManager* Report)
//{
//    UButton* Button = Cast<UButton>(Report->OverlayWidgetInstance->GetWidgetFromName(TEXT("ExitButton")));
//    // 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
//    Button->OnClicked.AddDynamic(this, &OffOverlay);
//    return Button;
//}