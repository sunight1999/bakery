
#include "AbnormalitiesReport/AbnormalityEvent.h"
#include "Widgets/AbnormalitiesReport/FadeImageWidget.h"
#include "General/BakeryGameMode.h"
#include "Blueprint/UserWidget.h"


AAbnormalityEvent::AAbnormalityEvent()
{
    PrimaryActorTick.bCanEverTick = false;
    FirstBlurEffect = false;
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
    switch (EventNum) {
    case 1:
        //1. 얼룩진 화면
        //TriggerBlurEffect();
        if (!FirstBlurEffect) {
            UE_LOG(LogTemp, Display, TEXT("위젯 생성 진행중 ..."));
            TriggerBlurEffectInitialize();
            FirstBlurEffect = true;
        }
        else {
            FadeLoop();
        }
        if (!EventPlay && FirstBlurEffect) {
            UE_LOG(LogTemp, Display, TEXT("위젯 이벤트 접근"));
            EventPlay = true;
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
    EventPlay = false;
}
void AAbnormalityEvent::FadeLoop()
{
    UE_LOG(LogTemp, Display, TEXT("fadeLoop진입"));
    GetWorld()->GetTimerManager().SetTimer(
        FadeLoopTimer,
        this,
        &AAbnormalityEvent::SetPlaceWidget,
        LoopInterval,
        true
    );
   
}
void AAbnormalityEvent::FadeLoopStop() {
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        GetWorld()->GetTimerManager().ClearTimer(WidgetInstance->GetLoopTimer());
        GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);
        WidgetInstance->StopAllAnimations();
        WidgetInstance->OffImage();
    }

}
void AAbnormalityEvent::SetPlaceWidget()
{
    FBox2D Test;
    int num = 0;
    
    for (int32 i = 0; i < FadeWidgetInstanceArray.Num(); i++) {
        UFadeImageWidget* CurrentWidget = FadeWidgetInstanceArray[i];
        CurrentWidget->LoopFadeImage();
        CurrentWidget->SetRandomImagePosition();
        //CurrentWidget->SetImagePos(FVector2D(-200,100));
        CurrentWidget->SetRandomImageScale();
        CurrentWidget->SetWidgetBounds();
        UE_LOG(LogTemp, Display, TEXT("%d번 박스 사이즈 %f %f %f %f"),i+1, CurrentWidget->Bound.Min.X, CurrentWidget->Bound.Min.Y, CurrentWidget->Bound.Max.X, CurrentWidget->Bound.Max.Y);
        UE_LOG(LogTemp, Display, TEXT("%d번 위치x : %f 위치y : %f 사이즈x : %f 사이즈y : %f"),i+1,CurrentWidget->BlurImage->GetRenderTransform().Translation.X,
        CurrentWidget->BlurImage->GetRenderTransform().Translation.Y, CurrentWidget->BlurImage->GetRenderTransform().Scale.X, CurrentWidget->BlurImage->GetRenderTransform().Scale.Y);
        bool flag = true;
        while (flag) {
            flag = false;
            for (int32 j = 0; j < i; j++) {
                UE_LOG(LogTemp, Display, TEXT("검증중"));
                if (CurrentWidget->GetWidgetBounds().Intersect(FadeWidgetInstanceArray[j]->GetWidgetBounds())) {
                    CurrentWidget->SetRandomImagePosition();
                    CurrentWidget->SetWidgetBounds();
                    flag = true;
                    UE_LOG(LogTemp, Display, TEXT("겹침 발생 재시도!! %d"),j);
                    break;
                }
            }
            
        }
        
    }
 /*   UFadeImageWidget* testWidget = FadeWidgetInstanceArray[2];
    for (UFadeImageWidget* WidgetInstance : FadeWidgetInstanceArray) {
        if (testWidget->GetWidgetBounds().Intersect(WidgetInstance->GetWidgetBounds()) == true) {
            UE_LOG(LogTemp, Display, TEXT("3번과 %d 겹침"), num + 1);
        }
        num++;
    }*/
}
/*
1. 이벤트 초기화 함수 만들어 둘 것 ( 끝나고 초기화 )
2. 
*/