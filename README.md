# 이상한 베이커리
이상한 세계에서 한국식 디저트 전문 베이커리를 운영해보자!

![2024-2 캡스톤디자인 경진대회 판넬 소프트웨어콘텐츠학과_이상한베이커리_1](https://github.com/user-attachments/assets/cb3ee3bc-39d9-499e-957f-bec37d44170d)

https://youtu.be/EfBH3FCLnr0

## 프로젝트 정보
### 언리얼 버전
5.4.4

### 주요 소스코드
#### 게임 로직 관련
|소스코드|설명|
|---|---|
|[General/BakeryGameMode](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/General/BakeryGameMode.h)|가게 운영 시작 및 종료 등 메인 로직 처리|
|[General/BakeryGameState](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/General/BakeryGameState.h)|게임 데이터 관리|
|[Bakery/HallManager](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Bakery/HallManager.h)|손님에게 빈 테이블 또는 대기 위치 할당|
|[Abnormality/AbnormalityManager](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Abnormality/AbnormalityManager.h)|이상현상 등록, 발생, 종료 관리|
|[Subsystems/UISubsystem](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Subsystems/UISubsystem.h)|UI 통합 관리 서브시스템|
|[Subsystems/SoundManager](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Subsystems/SoundManager.h)|효과음 관리 매니저 (싱글톤)|

#### 주요 액터 관련
|소스코드|설명|
|---|---|
|[Interactions/InteractorComponent](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Interactions/InteractorComponent.h)|상호작용, 잡기 등 상호작용 기능 부여|
|[Interactions/GrabberComponent](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Interactions/GrabberComponent.h)|물체 잡기 기능 부여|
|[Interactions/InteractableActor](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Interactions/Interactables/InteractableActor.h)|상호작용 가능한 액터 베이스 클래스. QuickSelectMenuType로 퀵메뉴 설정 가능|
|[Characters/PlayerPawn](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Characters/PlayerPawn.h)|플레이어 입력 처리 등|
|[Characters/Customer](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Characters/Customer.h)|손님 AI 처리 등|
|[Abnormality/AbnormalityEvents/AbnormalityEvent](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Abnormality/AbnormalityEvents/AbnormalityEvent.h)|이상현상 이벤트 액터 베이스 클래스|

#### 요리 관련
|소스코드|설명|
|---|---|
|[Kitchen/Ingredient](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Kitchen/Ingredient.h)|재료 액터. IngredientData로 재료 데이터, 메쉬 등 설정|
|[Kitchen/IngredientContainer](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Kitchen/IngredientContainer.h)|재료 보관함|
|[Kitchen/Countertop](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/Kitchen/Countertop.h)|조리대. AvailableCookingTools로 수행 가능한 조리법 설정 가능|
|[Kitchen/Data](https://github.com/sunight1999/bakery/tree/main/Source/Bakery/Public/Kitchen/Data)|재료, 레시피 데이터 |

#### 유틸 관련
|소스코드|설명|
|---|---|
|[General/Tools/Spawner](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/General/Tools/Spawner.h)|일정 주기로 지정한 액터 스폰|
|[General/Tools/CustomerSpawner](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/General/Tools/CustomerSpawner.h)|손님 액터를 스폰하고, 필요한 작업 추가 처리|
|[General/Tools/Pool](https://github.com/sunight1999/bakery/blob/main/Source/Bakery/Public/General/Tools/Pool.h)|템플릿 오브젝트 풀. UObject(Actor, Component)만 사용 가능|


___

![image](https://github.com/user-attachments/assets/6ff58963-c205-451c-a75d-9d79795be040)

___

![image](https://github.com/user-attachments/assets/61664708-951f-4ab6-a1fd-cf8340ddb320)

___

![image](https://github.com/user-attachments/assets/41a1afc5-d69f-493a-8bc5-5251d255d2cd)
