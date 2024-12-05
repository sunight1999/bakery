#include "Abnormality/AbnormalityManager.h"
#include "UObject/ConstructorHelpers.h"

#include "Subsystems/UISubsystem.h"
#include "General/BakeryGameMode.h"
#include "General/BakeryGameState.h"
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "Abnormality/AbnormalityEvents/Cause.h"
#include "Widgets/HUD/BakeryHUDWidget.h"

UAbnormalityManager* UAbnormalityManager::Instance = nullptr;

UAbnormalityManager::UAbnormalityManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> AbnormalityDataTableFinder(TEXT("/Game/Data/DT_AbnormalityEvent"));
	if (AbnormalityDataTableFinder.Succeeded())
	{
		UDataTable* AbnormalityDataTable = AbnormalityDataTableFinder.Object;
		TArray<const FAbnormalityEventData*> Datas;
		AbnormalityDataTable->GetAllRows(FString("Abnormalities"), Datas);

		for (auto Data : Datas)
		{
			AbnormalityEventDatas.Emplace(*Data);
		}
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
		Instance = NewObject<UAbnormalityManager>(InWorld, FName("AbnormalityManager")/*, EObjectFlags::RF_MarkAsRootSet*/);
		Instance->SetWorld(InWorld);
		Instance->AddToRoot();
	}

	return Instance;
}

// TODO: UAbnormalityManager, BakeryHUDWidget 관련해서 전반적으로 리팩토링 필요

void UAbnormalityManager::HandleRegisteredAbnormality(float ElapsedTime)
{
	ABakeryGameMode* BakeryGameMode = Cast<ABakeryGameMode>(GetWorld()->GetAuthGameMode());
	if (!BakeryGameMode->IsOpened())
	{
		return;
	}

	TSet<FName> Keys;
	OccurrenceTimeMap.GetKeys(Keys);

	for (const FName& AbnormalityName : Keys)
	{
		if (OccurrenceTimeMap[AbnormalityName] <= ElapsedTime)
		{
			ICause* AbnormalityEvent = Cast<ICause>(AbnormalityEventMap[AbnormalityName]);
			AbnormalityEvent->Cause();

			UUISubsystem* UISubsystem = World->GetGameInstance()->GetSubsystem<UUISubsystem>();
			UBakeryHUDWidget* HUDWidget = Cast<UBakeryHUDWidget>(UISubsystem->GetUIObject(FName("BakeryHUD")));
			HUDWidget->RemoveAbnormalityMarker(AbnormalityName);

			OccurrenceTimeMap.Remove(AbnormalityName);
			PendingFinishAbnormalities.Emplace(AbnormalityName);
		}
	}
}

void UAbnormalityManager::RegisterAbnormality(FName Name, int OccurrenceTime)
{
	// TODO: AbnormalityEventDatas를 맵으로 관리
	for (int32 i = 0; i < AbnormalityEventDatas.Num(); i++)
	{
		const FAbnormalityEventData& AbnormalityEventData = AbnormalityEventDatas[i];
		if (AbnormalityEventData.Name.Compare(Name) == 0)
		{
			// HUD 시간 바에 이상 현상 마커 등록
			UUISubsystem* UISubsystem = World->GetGameInstance()->GetSubsystem<UUISubsystem>();
			UBakeryHUDWidget* HUDWidget = Cast<UBakeryHUDWidget>(UISubsystem->GetUIObject(FName("BakeryHUD")));
			HUDWidget->AddAbnormalityMarker(AbnormalityEventData, OccurrenceTime);

			// 이상 현상 액터가 아직 생성되지 않았다면 생성
			AActor* AbnormalityEventActor = nullptr;
			if (!AbnormalityEventMap.Contains(AbnormalityEventData.Name))
			{
				AbnormalityEventActor = World->SpawnActor(AbnormalityEventData.AbnormalityEventActorClass);
				ICause* Cause = Cast<ICause>(AbnormalityEventActor);
				if (!Cause)
				{
					UE_LOG(LogTemp, Warning, TEXT("지정한 액터 클래스가 이상 현상 이벤트 인터페이스 ICause를 구현하지 않았습니다."));
					return;
				}

				AbnormalityEventMap.Emplace(AbnormalityEventData.Name, AbnormalityEventActor);
			}

			OccurrenceTimeMap.Emplace(AbnormalityEventData.Name, OccurrenceTime);

			break;
		}
	}
}

void UAbnormalityManager::RegisterFixedAbnormality()
{
	ClearRegisteredAbnormality();

	RegisterAbnormality(FName("DishThief"), 600);
	RegisterAbnormality(FName("Ink"), 680);
	RegisterAbnormality(FName("SpeedDown"), 690);
}

/*
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
	AActor* AbnormalityEventActor = nullptr;

	if (!AbnormalityEventMap.Contains(AbnormalityID))
	{
		AbnormalityEventActor = World->SpawnActor(AbnormalityEventData->AbnormalityEventActorClass);
		if (!AbnormalityEventActor->Implements< ICause>())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s에 지정한 액터 클래스는 이상 현상 이벤트 인터페이스 ICause를 구현하지 않았습니다."), *AbnormalityEventData->Name);
			return;
		}

		AbnormalityEventMap.Emplace(AbnormalityID, AbnormalityEventActor);
	}

	AbnormalityEventActor = AbnormalityEventMap[AbnormalityID];
	RegisteredAbnormalityEvents.Emplace(AbnormalityEventActor);
}
*/

void UAbnormalityManager::ClearRegisteredAbnormality()
{
	for (const FName& AbnormalityName : PendingFinishAbnormalities)
	{
		ICause* AbnormalityEvent = Cast<ICause>(AbnormalityEventMap[AbnormalityName]);
		AbnormalityEvent->Finish();
	}

	PendingFinishAbnormalities.Empty();
}