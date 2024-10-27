
#include "AbnormalitiesReport/AbnormalityEvent.h"
#include "Widgets/AbnormalitiesReport/FadeImageWidget.h"
#include "Blueprint/UserWidget.h"

UAbnormalityEvent::UAbnormalityEvent()
{
    ConstructorHelpers::FClassFinder<UFadeImageWidget> WidgetClassFinder(TEXT("/Game/Blueprints/Widgets/Abnormalities/WBP_FadeImage.WBP_FadeImage"));

    if (WidgetClassFinder.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("위젯 클래스 확인 완료."));
        FadeWidgetClass = WidgetClassFinder.Class;
    }
}

void UAbnormalityEvent::PlayEvent(int num)
{
}

void UAbnormalityEvent::TestPlayEvent()
{
    UE_LOG(LogTemp, Display, TEXT("TestPlayEvent진입!!!!!!!!!!!"));
    TriggerBlurEffectInitialize();
}

void UAbnormalityEvent::TriggerBlurEffect()
{
}
void UAbnormalityEvent::TriggerBlurEffectInitialize()
{
    UE_LOG(LogTemp, Warning, TEXT("초기화 접근 완료."));
    
    //for (int i = 0; i < MaxFadeArrayNum; i++) { //배열의 갯수만큼 초기화 - 할당 반복
        // 월드 가져오기
        //UWorld* World = GetWorld();
        //if (World)
        //{
        //    // 오버레이 위젯 생성 및 할당
        //    //FadeWidgetInstance = CreateWidget<UFadeImageWidget>(World, FadeWidgetClass,"FadeWidget");
        //    UE_LOG(LogTemp, Display, TEXT("FadeWidget출력!!!!!!!!!!!!!!!!"));
        //    FadeWidgetInstanceArray.Add(CreateWidget<UFadeImageWidget>(World, FadeWidgetClass, "FadeWidget"));
        //    //FadeWidgetInstanceArray.Add(FadeWidgetInstance);    // 배열에 생성된 인스턴스 추가
        //}
    //}
}
void UAbnormalityEvent::RandomlyPlaceImage(UStaticMeshComponent* Image, FVector2D MinBounds, FVector2D MaxBounds)
{
	int32 RandomImageNum = FMath::RandRange(0, 1);

}
