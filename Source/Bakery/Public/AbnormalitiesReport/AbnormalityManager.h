
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbnormalityManager.generated.h"

class AAbnormalitiesReportManager;
class UAbnormalityEvent;

//DECLARE_MULTICAST_DELEGATE(FAbnomalityReportDelegate);
//DECLARE_MULTICAST_DELEGATE_OneParam(FSetReportUIDelegate, FString);

UCLASS()
class BAKERY_API AAbnormalityManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbnormalityManager();
protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	int SituationNum = 0;
	int CreateRandomAnswer(int& number);
	UPROPERTY(EditAnywhere, Category = "UI")
	AAbnormalitiesReportManager* Report;
	UAbnormalityEvent* Event;
private:
	
};
/*
파일 연결

가져와야 할 정보
라운드 정보 변수
gamestate의 opened와 closed 델리게이트 활용
내가 가서 연결

이벤트 매니저와의 연결

플레이어 정보값 가져오기
캐릭터무브먼트컴포넌트->MaxAccelertaion같이 접근하면 되긴 함

플레이어 관련 변수 파악하기

라운드 타이머 : 베이커리게임모드에 있음
gamemode -> CurrentOprationgTimes  현재 영업시간
OprationgTimes 총 영업시간

라운드값을 델리게이트에서 매니저를  호출하게끔 함

랜덤값 정하기

db값 가져오기
UI 생성하기
호출하기

이벤트 매니저에 랜덤값 넘기기
랜덤값에 따라 이벤트 접근하기
영업시간 받아오기
영업시간에 맞춰 이벤트 실행하기

델리게이트 클로즈


*/