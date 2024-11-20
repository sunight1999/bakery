#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cause.h"
#include "AbnormalityEvent.generated.h"

/*
 * 이상현상 데이터 테이블 구조체
 */
USTRUCT(BlueprintType)
struct FAbnormalityEventData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 이상현상 이름
	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	FString Name;

	// 이상현상 예보 수신인
	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	FString ForecastLetterAddressee = TEXT("이상한 베이커리");

	// 이상현상 예보 발신인
	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	FString ForecastLetterAddresser = TEXT("이상청");

	// 이상현상 예보문
	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent|Forecast")
	FString ForecastLetter;
	
	// 이상현상 이벤트 구현 액터 (반드시 ICause를 구현해야 함)
	UPROPERTY(EditAnywhere, Category = "AbnormalityEvent")
	TSubclassOf<AActor> AbnormalityEventActorClass;
};

/*
 * 이상현상 이벤트
 */
UCLASS()
class BAKERY_API AAbnormalityEvent : public AActor, public ICause
{
	GENERATED_BODY()
public:
	AAbnormalityEvent();
	virtual void Tick(float DeltaTime) override;

	virtual void Cause() override {}
	virtual void Finish() override {}

	const FAbnormalityEventData* GetAbnormalityEventData() const { return AbnormalityEventData; }
	void SetAbnormalityEventData(FAbnormalityEventData* InAbnormalityEventData) { AbnormalityEventData = InAbnormalityEventData; }

protected:
	virtual void BeginPlay() override;

private:
	const FAbnormalityEventData* AbnormalityEventData;
};
 