#include "AbnormalitiesReport/AbnormalityManager.h"
#include "AbnormalitiesReport/AbnormalitiesReportManager.h"
#include "AbnormalitiesReport/AbnormalityEvent.h"
#include <Kismet/GameplayStatics.h>

AAbnormalityManager::AAbnormalityManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAbnormalityManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundReport;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAbnormalitiesReportManager::StaticClass(),FoundReport); // AbnomalityReport 변수 검색 및 할당
	if (FoundReport.Num() > 0) {
		Report = Cast<AAbnormalitiesReportManager>(FoundReport[0]);
	}
	Report->CreateOverlayWidget();
	CreateRandomAnswer(SituationNum);
	Report->GetRandomAnswer(SituationNum);
	Event->TestPlayEvent();
}

void AAbnormalityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AAbnormalityManager::CreateRandomAnswer(int& number)
{
	number = FMath::RandRange(0, 6);
	return number;
}

