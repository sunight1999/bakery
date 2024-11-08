
#include "AbnormalitiesReport/AbnormalityEvent.h"
#include "Widgets/AbnormalitiesReport/FadeImageWidget.h"
#include "Blueprint/UserWidget.h"


void AAbnormalityEvent::PlayEvent(int num)
{
}
void AAbnormalityEvent::TestPlayEvent()
{
    UE_LOG(LogTemp, Display, TEXT("TestPlayEvent진입!!!!!!!!!!!"));
    TriggerBlurEffectInitialize();
}
void AAbnormalityEvent::TriggerBlurEffect()
{
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        //WidgetInstance->
    }
}
void AAbnormalityEvent::TriggerBlurEffectInitialize()
{
    UE_LOG(LogTemp, Warning, TEXT("초기화 접근 완료."));
   for (int i = 0; i < MaxFadeArrayNum; i++) { //배열의 갯수만큼 초기화 - 할당 반복
        // 월드 가져오기
        UWorld* World = GetWorld();
        if (World)
        {
            // 오버레이 위젯 생성 및 할당
            //FadeWidgetInstance = CreateWidget<UFadeImageWidget>(World, FadeWidgetClass);
            UE_LOG(LogTemp, Display, TEXT("FadeWidget출력!!!!!!!!!!!!!!!!"));
            FadeWidgetInstanceArray.Add(CreateWidget<UFadeImageWidget>(World, FadeWidgetClass[i],FName("FadeWidget %d",i)));
            for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
                WidgetInstance->AddToViewport();
                WidgetInstance->SetRandomImagePosition(RandomPosMin,RandomPosMax);
                WidgetInstance->SetRandomImageScale(RandomSizeMin,RandomSizeMax);
                WidgetInstance->OffImage();
                WidgetInstance->OnFadeImage();
            }
           // FadeWidgetInstanceArray.Add(FadeWidgetInstance);    // 배열에 생성된 인스턴스 추가
        }
   }
}

