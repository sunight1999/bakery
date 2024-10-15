#include "AbnormalitiesReport/AbnormalitiesReport.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Widgets/AbnormalitiesReport/AbnormalitiesReportWidget.h"
#include "Components/WidgetComponent.h"
#include <General/BakeryGameMode.h>
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
}void AAbnormalitiesReport::BeginPlay()
{
    Super::BeginPlay();
    if (IsCreate) {
        CreateOverlayWidget();
        SituationCoolDown();
    }
}void AAbnormalitiesReport::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void AAbnormalitiesReport::CreateOverlayWidget()
{
    if (OverlayWidgetClass != NULL)
    {
        // 월드 가져오기
        UWorld* World = GetWorld();
        if (World)
        {
            // 오버레이 위젯 생성
            OverlayWidgetInstance = CreateWidget<UAbnormalitiesReportWidget>(World, OverlayWidgetClass);
            //Random = FMath::RandRange(1, Situation);;
            if (OverlayWidgetInstance)
            {
                // 위젯을 화면에 추가
                OverlayWidgetInstance->AddToViewport();
                OverlayWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
                OverlayWidgetInstance->ExitButtonBinding();
                OverlayWidgetInstance->ChangeReportText("asd");
                SituationCoolDown();

            }
        }
    }
    else {
        UE_LOG(LogTemp, Display, TEXT("위젯이 할당되지 않았습니다. 위젯을 할당해 주세요."));
    }
}void AAbnormalitiesReport::SituationCoolDown()
{
    GetWorldTimerManager().SetTimer(
        CooldownTimerHandle, this,
        &AAbnormalitiesReport::SetFuncFlag, CooldownTime,
        true, 0);
}
void AAbnormalitiesReport::SetFuncFlag()
{
    if (IsView) {
        CreateRandomAnswer();
        OverlayWidgetInstance->OnOverlay();
        ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
    }
}void AAbnormalitiesReport::CreateRandomAnswer() ///확률 시작
{
    int32 RandomNum = FMath::RandRange(0, 6);
    FSituationData* Row = DataTable->FindRow<FSituationData>(*FString::FromInt(RandomNum), "데이터테이블 다운 실패");
    if (Row) {
        FString HeadName = Row->Situation;
        FString Name = Row->Explanation;
        OverlayWidgetInstance->ChangeReportHeadText(HeadName); // 행이 우선
        OverlayWidgetInstance->ChangeReportText(Name); // 행이 우선
    }
}// 코드 가동 순서
//1.데이터 테이블 연결하기(미완)
//2.array 생성 후 정렬(미완)
//3.랜덤 값 추출(해결)
//4.출력(해결)