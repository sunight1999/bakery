
#include "AbnormalitiesReport/AbnormalityEvent.h"
#include "Widgets/AbnormalitiesReport/FadeImageWidget.h"
#include "General/BakeryGameMode.h"
#include "Blueprint/UserWidget.h"


AAbnormalityEvent::AAbnormalityEvent()
{
    PrimaryActorTick.bCanEverTick = false;
    EventPlay = false;
    EventNum = 0;
    
}

void AAbnormalityEvent::BeginPlay()
{
    
    
}

void AAbnormalityEvent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAbnormalityEvent::PlayEvent(int num)
{
    
}
void AAbnormalityEvent::ChoiceEvent() {
    BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode()); // 게임모드 가져오기 
    EventPlay = true;
    switch (EventNum) {
    case 1:
        //1. 얼룩진 화면
        //TriggerBlurEffect();
        if (FirstBlurEffect==true) {
            TriggerBlurEffectInitialize();
            FirstBlurEffect = false;
        }
        else {
            FadeLoop();
        }
        if (EventPlay) {
            GetWorld()->GetTimerManager().SetTimer(
                RoundTimerHandle,                // 타이머 핸들
                this,                       // 호출할 객체
                &AAbnormalityEvent::TriggerBlurEffectStop,           // 호출할 함수
                BakeryGameMode->GetOperatingTime(),                     // 호출 간격 (초 단위)
                false                         // 반복 여부 (true면 반복, false면 한 번만 실행)
            );
        }
        break;
        default:
            UE_LOG(LogTemp, Display, TEXT("미완성 이벤트 또는 오류입니다."));
            break;
    }
    
}
void AAbnormalityEvent::TestPlayEvent()
{
    UE_LOG(LogTemp, Display, TEXT("TestPlayEvent진입!!!!!!!!!!!"));
    //TriggerBlurEffectInitialize();
}

void AAbnormalityEvent::SetEventNum(int num)
{
    EventNum = num;
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
            FadeWidgetInstanceArray.Add(CreateWidget<UFadeImageWidget>(World, FadeWidgetClass[i], FName("FadeWidget %d", i)));
            // FadeWidgetInstanceArray.Add(FadeWidgetInstance);    // 배열에 생성된 인스턴스 추가
        }
    }
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        WidgetInstance->AddToViewport();
        WidgetInstance->SetRandomPosMinMax(RandomPosMin, RandomPosMax);
        WidgetInstance->SetRandomSizeMinMax(RandomSizeMin, RandomSizeMax);
        WidgetInstance->SetRandomImagePosition();
        WidgetInstance->SetRandomImageScale();
        WidgetInstance->OffImage();
    }
    FadeLoop();
}

void AAbnormalityEvent::TriggerBlurEffectStop()
{
    FadeLoopStop();
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        WidgetInstance->OffImage();
    }
    FadeLoop();
    EventPlay = false;
}
void AAbnormalityEvent::FadeLoop()
{
    GetWorld()->GetTimerManager().SetTimer(
        FadeLoopTimer,
        this,
        &AAbnormalityEvent::SetPlaceWidget,
        FadeWidgetInstance->LoopInterval,
        true
    );
}
void AAbnormalityEvent::FadeLoopStop() {
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        WidgetInstance->OffImage();
    }
    GetWorld()->GetTimerManager().ClearTimer(FadeLoopTimer);
}
void AAbnormalityEvent::SetPlaceWidget()
{
    FadeWidgetInstanceArray[0]->SetRandomImagePosition();
    for (int32 i = 1; i < FadeWidgetInstanceArray.Num(); i++) {
        for (int32 j = 0; j < i; j++) {
            if (i == j) continue;
            FadeWidgetInstanceArray[i]->SetRandomImagePosition();
            if (FadeWidgetInstanceArray[i]->Bound.Intersect(FadeWidgetInstanceArray[j]->Bound)) {
                j = 0;
            }
            else {
                continue;
            }
        }
    }
    /*
    int i = 0;
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        WidgetBoxArray[i] = WidgetInstance->GetWidgetBounds(WidgetInstance);
        i++;
    }
    for (int32 i = 1; i < WidgetBoxArray.Num(); i++)
    {
        for (int32 j = 0; j < i; j++)
        {
            // 자기 자신과 비교를 피하기 위해 i와 j가 같을 경우 건너뜁니다.
            if (i == j) continue;
            
            if(WidgetBoxArray[i].Intersect(WidgetBoxArray[j])){
                FadeWidgetInstanceArray[j]->SetRandomImagePosition();
                WidgetBoxArray[j] = FadeWidgetInstanceArray[j]->GetWidgetBounds(FadeWidgetInstanceArray[j]);
                i = 0;
            }
        }
    }*/
}
void AAbnormalityEvent::WidgetArrayBounds() {

}
/*
1. 이벤트 초기화 함수 만들어 둘 것 ( 끝나고 초기화 )
2. 
*/