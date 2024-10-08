
#include "AbnormalitiesReport/AbnormalitiesReport.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TimerManager.h"


AAbnormalitiesReport::AAbnormalitiesReport()
{
	PrimaryActorTick.bCanEverTick = true;
    ConstructorHelpers::FObjectFinder<UDataTable> DialogueDataTableFinder(TEXT("/Game/Data/Situation/SituationData"));
    if (DialogueDataTableFinder.Succeeded())
    {
        UE_LOG(LogTemp, Display, TEXT("데이터 접근 성공"));

        DataTable = DialogueDataTableFinder.Object;
    }
    else {
        UE_LOG(LogTemp, Display, TEXT("데이터 접근 실패!!"));
    }
}
void AAbnormalitiesReport::BeginPlay()
{
    Super::BeginPlay();
    if (IsCreate) {
        CreateOverlayWidget();
        SituationCoolDown();
    }
}
void AAbnormalitiesReport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbnormalitiesReport::CreateOverlayWidget()
{
    if (OverlayWidgetClass)
    {
        // 월드 가져오기
        UWorld* World = GetWorld();
        if (World)
        {
            // 오버레이 위젯 생성
            OverlayWidgetInstance = CreateWidget<UUserWidget>(World, OverlayWidgetClass);
            //Random = FMath::RandRange(1, Situation);;

            if (OverlayWidgetInstance)
            {
                // 위젯을 화면에 추가
                OverlayWidgetInstance->AddToViewport();
                OverlayWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

                UWidget* TextBlock1 = (OverlayWidgetInstance->GetWidgetFromName(TEXT("TextBlock")));
                TextBlock = Cast<UTextBlock>(TextBlock1);
                UWidget* Button1 = (OverlayWidgetInstance->GetWidgetFromName(TEXT("ExitButton")));
                ExitButton = Cast<UButton>(Button1);
                // 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
                ExitButton->OnClicked.AddDynamic(this, &AAbnormalitiesReport::OffOverlay);
                //MyActor->SetActorHiddenInGame(true);

                    if (TextBlock) {
                        ChangeText("상황 보고가 들어갈 예정입니다.");
                    }
            }
        }
    }
}
void AAbnormalitiesReport::SituationCoolDown()
{
    GetWorldTimerManager().SetTimer(
        CooldownTimerHandle, this,
        &AAbnormalitiesReport::SetFuncFlag, CooldownTime,
        true, 0);
}
void AAbnormalitiesReport::OnOverlay()
{
    OverlayWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    CreateRandomAnswer();
}

void AAbnormalitiesReport::OffOverlay()
{       
    OverlayWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}
void AAbnormalitiesReport::ChangeText(FString Text)
{
    if (TextBlock)
        TextBlock->SetText(FText::FromString(Text));
}
void AAbnormalitiesReport::SetFuncFlag()
{
    if (IsView) {
        OnOverlay();
    }
}

void AAbnormalitiesReport::CreateRandomAnswer() ///확률 시작
{
    int32 RandomNum = FMath::RandRange(0, 6);
    
    FSituationData* Row = DataTable->FindRow<FSituationData>(*FString::FromInt(RandomNum),"");
    if (Row) {
        FString Name = Row->Situation;
        ChangeText(Name); // 행이 우선
    }
}


// 코드 가동 순서
//1.데이터 테이블 연결하기(미완)
//2.array 생성 후 정렬(미완)
//3.랜덤 값 추출(해결)
//4.출력(해결)


