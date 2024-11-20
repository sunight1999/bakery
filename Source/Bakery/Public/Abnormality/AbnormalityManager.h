
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

	void RegisterRandomAbnormality();
	void CauseOneAbnormality();
	void CauseAllAbnormality();
	void ClearRegisteredAbnormality();

private:
	static UAbnormalityManager* Instance;
	UWorld* World;

	// 전체 이상 현상 데이터
	TArray<const FAbnormalityEventData*> AbnormalityEventDatas;
	TMap<int32, ICause*> AbnormalityEventMap;

	// 현재 발생 예정인 이상 현상
	TSet<ICause*> RegisteredAbnormalityEvents;
};