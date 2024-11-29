
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalityManager.generated.h"

struct FAbnormalityEventData;
class ICause;

UCLASS()
class BAKERY_API UAbnormalityManager : public UObject
{
	GENERATED_BODY()
	
public:	
	UAbnormalityManager();
	~UAbnormalityManager();
	static UAbnormalityManager* GetInstance(UWorld* InWorld);

	void SetWorld(UWorld* InWorld) { World = InWorld; }

	void HandleRegisteredAbnormality(float ElapsedTime);

	void RegisterAbnormality(FName Name, int OccurrenceTime);
	void RegisterFixedAbnormality();
	//void RegisterRandomAbnormality();
	void ClearRegisteredAbnormality();

private:
	static UAbnormalityManager* Instance;
	UWorld* World;

	// 전체 이상 현상 데이터
	TArray<const FAbnormalityEventData*> AbnormalityEventDatas;

	// 액터로 생성된 이상 현상 데이터
	TMap<FName, AActor*> AbnormalityEventMap;

	// 현재 발생 예정인 이상 현상
	TMap<const FAbnormalityEventData*, float> OccurrenceTimeMap;
};