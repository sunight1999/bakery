#include "Abnormality/AbnormalityManager.h"
#include "UObject/ConstructorHelpers.h"

#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "Abnormality/AbnormalityEvents/Cause.h"

UAbnormalityManager* UAbnormalityManager::Instance = nullptr;

UAbnormalityManager::UAbnormalityManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> AbnormalityDataTableFinder(TEXT("/Game/Data/DT_AbnormalityEvent"));
	if (AbnormalityDataTableFinder.Succeeded())
	{
		UDataTable* AbnormalityDataTable = AbnormalityDataTableFinder.Object;
		AbnormalityDataTable->GetAllRows(FString("Abnormalities"), AbnormalityEventDatas);
	}
}

UAbnormalityManager::~UAbnormalityManager()
{
	Instance = nullptr;
	World = nullptr;
}

UAbnormalityManager* UAbnormalityManager::GetInstance(UWorld* InWorld)
{
	if (!Instance)
	{
		Instance = NewObject<UAbnormalityManager>(InWorld);
		Instance->SetWorld(InWorld);
	}

	return Instance;
}

void UAbnormalityManager::RegisterRandomAbnormality()
{
	ClearRegisteredAbnormality();

	if (AbnormalityEventDatas.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("이상 현상 이벤트 정보가 없습니다. 데이터 테이블(Data/DT_AbnormalityEvent)을 확인해주세요."));
		return;
	}

	int32 AbnormalityID = FMath::RandRange(0, AbnormalityEventDatas.Num() - 1);
	const FAbnormalityEventData* AbnormalityEventData = AbnormalityEventDatas[AbnormalityID];
	ICause* AbnormalityEvent = nullptr;

	if (!AbnormalityEventMap.Contains(AbnormalityID))
	{
		AActor* Actor = World->SpawnActor(AbnormalityEventData->AbnormalityEventActorClass);
		AbnormalityEvent = Cast<ICause>(Actor);
		if (!AbnormalityEvent)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s에 지정한 액터 클래스는 이상 현상 이벤트 인터페이스 ICause를 구현하지 않았습니다."), *AbnormalityEventData->Name);
			return;
		}

		AbnormalityEventMap.Emplace(AbnormalityID, AbnormalityEvent);
	}

	AbnormalityEvent = AbnormalityEventMap[AbnormalityID];
	RegisteredAbnormalityEvents.Emplace(AbnormalityEvent);
}

void UAbnormalityManager::CauseOneAbnormality()
{
	(*RegisteredAbnormalityEvents.begin())->Cause();
}

void UAbnormalityManager::CauseAllAbnormality()
{
	for (ICause* AbnormalityEvent : RegisteredAbnormalityEvents)
	{
		AbnormalityEvent->Cause();
	}
}

void UAbnormalityManager::ClearRegisteredAbnormality()
{
	for (ICause* AbnormalityEvent : RegisteredAbnormalityEvents)
	{
		AbnormalityEvent->Finish();
	}

	RegisteredAbnormalityEvents.Empty();
}