
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

        FString SituationDataString = "Situation";
        TArray<FSituationData*> SituationPtrArray;
        DataTable->GetAllRows<FSituationData>(SituationDataString, SituationPtrArray);

       for (FSituationData* Data : SituationPtrArray)
       {
           SituationArray.Add(*Data);
       }
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
    }
    SituationCoolDown();
    //Database = CreateDefaultSubobject<UAbonomalitiesReportComponent>(TEXT("SituationData"));
    

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
                //OverlayWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

                UWidget* TextBlock1 = (OverlayWidgetInstance->GetWidgetFromName(TEXT("TextBlock")));
                TextBlock = Cast<UTextBlock>(TextBlock1);
                UWidget* Button1 = (OverlayWidgetInstance->GetWidgetFromName(TEXT("ExitButton")));
                ExitButton = Cast<UButton>(Button1);
                // 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
                ExitButton->OnClicked.AddDynamic(this, &AAbnormalitiesReport::OffOverlay);
                //MyActor->SetActorHiddenInGame(true);

                    if (TextBlock) {
                        ChangeText("Test1");
                    }
                
            }
        }
    }
}
void AAbnormalitiesReport::SituationCoolDown()
{
    //GetWorldTimerManager().SetTimer(
    //    CooldownTimerHandle, this,
    //    &AAbnormalitiesReport::TestPrint, CooldownTime,
    //    true, 0);
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
void AAbnormalitiesReport::TestPrint()
{
    UE_LOG(LogTemp, Display, TEXT("YEEEEEEEEE"));
    UE_LOG(LogTemp, Display, TEXT("YEEEEEEEEE"));
    UE_LOG(LogTemp, Display, TEXT("YEEEEEEEEE"));

}
void AAbnormalitiesReport::ChangeText(FString Text)
{
    if (TextBlock)
        TextBlock->SetText(FText::FromString(Text));
}
void AAbnormalitiesReport::SetFuncFlag()
{
    if (IsProbability == true) {
        //CreateRandomAnswer()  ;
    }
    else{
        OnOverlay();
    }
}

void AAbnormalitiesReport::CreateRandomAnswer() ///확률 시작
{   
    
    int32 RandomNum = FMath::RandRange(0, 6);
    ChangeText(SituationArray[RandomNum].Situation); // 행이 우선
        

    // 데이터 테이블에서 해당 행을 찾음
    //FString SecondColumnValue = DialogueDataTable->FindRow(RowName,);
    //ChangeText(DialogueDataTable->GetTableAsCSV())
    //Probability = FMath::Clamp(Probability, 0.0f, 100.0f); // 확률 범위 제한 (0에서 100 사이로 제한)
    //srand((unsigned int)time(NULL)); // 난수 초기화
    // 
    //int randnum = rand() % 100000 / 100.f; // 난수 생성
    //std::random_device rd;
    //std::mt19937 mt(rd()); //난수 생성기
    //std::uniform_int_distribution<int> dist(0, 99);
    //auto randNum = dist(mt);
}


// 코드 가동 순서
//1.데이터 테이블 연결하기(미완)
//2.array 생성 후 정렬(미완)
//3.랜덤 값 추출(해결)
//4.출력(해결)


