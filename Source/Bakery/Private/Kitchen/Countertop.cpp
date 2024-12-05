// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Countertop.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Interactions/Interactables/InteractableComponent.h"
#include "Widgets/Progress/ProgressWidget.h"

#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/IngredientContainer.h"
#include "Kitchen/Dish.h"
#include "Kitchen/Data/IngredientData.h"
#include "Subsystems/RecipeSubsystem.h"
#include "Characters/PlayerPawn.h"
#include "Subsystems/SoundManager.h"
#include "Indicators/CookingStateIndicator.h"

#include "Subsystems/UISubsystem.h"

ACountertop::ACountertop()
{
	PrimaryActorTick.bCanEverTick = true;

	BottomSupportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomSupportMesh"));
	BottomSupportMesh->SetupAttachment(InteractionBox);

	TopPlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopPlateMesh"));
	TopPlateMesh->SetupAttachment(BottomSupportMesh);

	OptionalToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OptionalToolMesh"));
	OptionalToolMesh->SetupAttachment(TopPlateMesh);

	KeepPoint = CreateDefaultSubobject<USceneComponent>(TEXT("KeepPoint"));
	KeepPoint->SetupAttachment(TopPlateMesh);

	CookingStateIndicatorPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CookingStateIndicatorPoint"));
	CookingStateIndicatorPoint->SetupAttachment(RootComponent);
	CookingStateIndicatorPoint->SetRelativeLocation(FVector(0.f, 0.f, 250.f));

	CookingProgressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CookingProgressWidget"));
	CookingProgressWidget->SetupAttachment(InteractionBox);
	CookingProgressWidget->SetVisibility(false);

	PrimaryCookingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PrimaryCookingEffect"));
	PrimaryCookingEffect->SetAutoActivate(false);
	PrimaryCookingEffect->SetupAttachment(InteractionBox);
}

void ACountertop::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	RecipeSubsystem = GameInstance->GetSubsystem<URecipeSubsystem>();

	verify(RecipeSubsystem);

	CookingProgress = Cast<UProgressWidget>(CookingProgressWidget->GetUserWidgetObject());

	// 요리 상태 Indicator 스폰
	verify(CookingStateIndicatorClass);

	FVector Location = CookingStateIndicatorPoint->GetComponentLocation();
	FVector Scale = CookingStateIndicatorPoint->GetComponentScale();
	CookingStateIndicator = Cast<ACookingStateIndicator>(GetWorld()->SpawnActor(CookingStateIndicatorClass));
	CookingStateIndicator->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CookingStateIndicator->Initailize(Location, Scale);
	CookingStateIndicator->SetActorRotation(CookingStateFixedRotation);

	if (bHasQuickMenu)
	{
		verify(QuickMenuValues.Num() > 0);
		CurrentCookingTool = QuickMenuValues[0];
	}
}

void ACountertop::Tick(float DeltaTime)
{
	if (bIsCooking && bIsAutomatic)
	{
		CurrentAutoCookingTime += DeltaTime;
		CookingProgress->SetPercentage(CurrentAutoCookingTime / AutoCookingTime);

		if (IsCookingDone())
		{
			EndCook();
		}
	}
}

void ACountertop::ResetCooking(bool bFullyReset)
{
	bIsCooking = false;
	CurrentAutoCookingTime = 0.f;
	CurrentHandCookingTime = 0;

	if (!bHasQuickMenu)
	{
		CurrentCookingTool = ECookingTool::None;
	}

	CookingProgressWidget->SetVisibility(false);
	CookingProgress->SetPercentage(0.f);

	StopCookingAnimation();

	if (IsValid(CurrentAudio))
	{
		CurrentAudio->Stop();
		CurrentAudio = nullptr;
	}

	if (bFullyReset)
	{
		CurrentKeptObject = nullptr;
		CurrentKeptIngredient = nullptr;
	}
}

/*
 * 요리 관련 함수
 */
void ACountertop::BeginCook(ECookingTool CookingTool)
{
	CurrentCookingTool = CookingTool;

	CookingProgressWidget->SetVisibility(true);
	CookingStateIndicator->Hide();

	// 요리 이펙트 및 애니메이션 재생
	PlayCookingAnimation();


	bIsCooking = true;
	Cook();
}

void ACountertop::Cook()
{
	if (!bIsCooking)
	{
		return;
	}
	
	// 효과음 재생
	if (!CurrentAudio)
	{
		USoundManager* SoundManager = USoundManager::GetInstance(GetWorld());
		const FName* CookingSoundTag = CurrentKeptIngredient->GetIngredientData()->GetCookingSoundTag(CurrentCookingTool);
		const FVector& CookingSoundLocation = GetActorLocation();

		if (CookingSoundTag)
		{
			CurrentAudio = SoundManager->PlaySoundAtLocationByTag(*CookingSoundTag, CookingSoundLocation);
		}
	}

	// 요리 상호작용 횟수 증가 또는 시간 단축
	if (bIsAutomatic)
	{
		CurrentAutoCookingTime += CalculateAutoShorten();
	}
	else
	{
		++CurrentHandCookingTime;
		CookingProgress->SetPercentage((float)CurrentHandCookingTime / (float)HandCookingTime);

		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		PlayerPawn->HandsCook();
	}

	if (IsCookingDone())
	{
		EndCook();
	}
}

void ACountertop::EndCook()
{
	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("CookingDone"), GetActorLocation());

	const UIngredientData* CurrentKeptIngredientData = CurrentKeptIngredient->GetIngredientData();
	if (!CurrentKeptIngredientData->IsPendingCooking())
	{
		const UIngredientData* TargetIngredientData = CurrentKeptIngredientData->CheckCookable(CurrentCookingTool);
		CurrentKeptIngredient->ChangeIngredient(TargetIngredientData);
	}
	else
	{
		CurrentKeptIngredient->SetIsCooked(true);
	}

	CookingStateIndicator->Show(ECookingIndicator::Done);
	ResetCooking();

	// 더 이상 요리가 불가능 할 때까지 자동으로 요리 진행
	if (bIsAutomatic)
	{
		HandleCook();
	}
}

void ACountertop::PlayCookingAnimation()
{
	if (PrimaryCookingEffect)
	{
		PrimaryCookingEffect->Activate();
	}

	if (CurrentKeptIngredient)
	{
		CurrentKeptIngredient->PlayBeingCookedAnimation(CurrentCookingTool);
	}

	OnCookingAnimStarted.Broadcast();
}

void ACountertop::StopCookingAnimation()
{
	if (PrimaryCookingEffect)
	{
		PrimaryCookingEffect->Deactivate();
	}

	if (CurrentKeptIngredient)
	{
		CurrentKeptIngredient->StopBeingCookedAnimation();
	}

	OnCookingAnimStopped.Broadcast();
}

void ACountertop::HandleCook()
{
	if (PendingGrabber)
	{
		UPrimitiveComponent* Grabbed = PendingGrabber->GetGrabbed();
		AIngredient* GrabbedIngredient = nullptr;
		if (Grabbed)
		{
			GrabbedIngredient = Cast<AIngredient>(Grabbed->GetOwner());
		}

		// 플레이어가 잡고 있는 액터가 재료일 경우 재료 합치기 시도
		if (GrabbedIngredient && CurrentKeptIngredient)
		{
			const UIngredientData* MergedIngredientData = CurrentKeptIngredient->TryMergeIngredient(GrabbedIngredient);
			if (MergedIngredientData)
			{
				PendingGrabber->Release();

				// TODO: 재료 풀링을 각 재료 보관함이 아니라 하나의 오브젝트가 수행하도록 수정
				for (TActorIterator<AIngredientContainer>It(GetWorld()); It; ++It)
				{
					if ((*It)->TryReturnIngredient(GrabbedIngredient))
					{
						break;
					}
				}

				CurrentKeptIngredient->ChangeIngredient(MergedIngredientData);

				// 합치기에 성공한 경우 요리를 다시 시작해야하므로 reset 수행
				ResetCooking();
			}
		}
	}

	// 진행 중인 요리가 있다면 요리 진행
	if (bIsCooking)
	{
		Cook();
		return;
	}

	if (!CurrentKeptIngredient || CurrentKeptIngredient->IsCooked())
	{
		return;
	}

	// 현재 Countertop에서 사용 가능한 요리 도구 중 CurrentKeptIngredient와 맞는 게 있으면 요리 시작
	const UIngredientData* IngredientData = CurrentKeptIngredient->GetIngredientData();
	if (bHasQuickMenu)
	{
		if (const UIngredientData* CookingTarget = IngredientData->CheckCookable(CurrentCookingTool))
		{
			BeginCook(CurrentCookingTool);
			return;
		}
	}
	else
	{
		for (ECookingTool CookingTool : AvailableCookingTools)
		{
			if (const UIngredientData* CookingTarget = IngredientData->CheckCookable(CookingTool))
			{
				BeginCook(CookingTool);
				return;
			}
		}
	}
}

void ACountertop::HandleQuickMenu()
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerPawn->SetQuickMenu(QuickSelectMenuType);
	PlayerPawn->ShowQuickMenu(CurrentQuickMenuIndex);
}

void ACountertop::HandleQuickMenuClose()
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	int SelectedItemIndex = PlayerPawn->HideQuickMenu();
	if (SelectedItemIndex >= QuickMenuValues.Num())
	{
		SelectedItemIndex = 0;
	}

	CurrentQuickMenuIndex = SelectedItemIndex;
	CurrentCookingTool = QuickMenuValues[CurrentQuickMenuIndex];
	bIsQuickMenuOpened = false;

	ResetCooking();
}

/*
 * Interaction Functions
 */
void ACountertop::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	// FInteractionInfo::NoneInteraction를 사용하는 경우가 있어 Interactor 유효성 검사 필요
	if (InteractionInfo.Interactor)
	{
		PendingGrabber = InteractionInfo.Interactor->GetGrabber();
	}

	InteractPressedTime = 0.f;
}

void ACountertop::OnInteract(float DeltaTime)
{
	if (bHasQuickMenu && !bIsQuickMenuOpened)
	{
		InteractPressedTime += DeltaTime;

		if (bHasQuickMenu && InteractPressedTime >= QuickMenuShowDelay)
		{
			HandleQuickMenu();
			bIsQuickMenuOpened = true;
		}
	}
}

void ACountertop::OnExitInteract()
{
	if (bIsQuickMenuOpened)
	{
		HandleQuickMenuClose();
	}
	else
	{
		HandleCook();
	}

	PendingGrabber = nullptr;
}

/*
 * Grab Interaction Functions
 */
void ACountertop::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	// 현재 Countertop이 보관 중인 액터가 없으면, 플레이어의 Grabber가 쥐고 있는 액터를 가져와 KeepPoint에 올림
	if (!CurrentKeptObject)
	{
		// Grabber에 액터가 없다면 더 이상 수행할 작업이 없으므로 종료
		if (!Grabbed)
		{
			return;
		}

		AActor* GrabbedActor = Grabbed->GetOwner();
		AIngredient* GrabbedIngredient = Cast<AIngredient>(GrabbedActor);

		// 조리대 위에 올려진 조리 도구가 있을 경우 조리 외에는 물체를 올려놓지 못하도록 처리
		if (OptionalToolMesh->GetStaticMesh())
		{
			if (!GrabbedIngredient)
			{
				return;
			}

			bool bIsCookable = false;
			for (ECookingTool CookingTool : AvailableCookingTools)
			{
				if (const UIngredientData* CookingTarget = GrabbedIngredient->GetIngredientData()->CheckCookable(CookingTool))
				{
					bIsCookable = true;
					break;
				}
			}

			if (!bIsCookable)
			{
				if (bIsPendingCookingCompletable)
				{
					bIsCookable = GrabbedIngredient->GetIngredientData()->CheckCookable(ECookingTool::Put) != nullptr;
				}
			}

			if (!bIsCookable)
			{
				return;
			}
		}

		CurrentKeptObject = Grabbed->GetOwner();
		CurrentKeptIngredient = Cast<AIngredient>(CurrentKeptObject);

		// 오브젝트 위치 설정
		FVector NewLocation = KeepPoint->GetComponentLocation();
		FRotator NewRotation = CurrentKeptObject->GetActorRotation();
		NewRotation.Pitch = 0.f;

		Grabber->Release();

		// 충돌 오류 방지를 위해 Collision과 Physics 비활성화
		Grabbed->SetSimulatePhysics(false);
		CurrentKeptObject->SetActorLocationAndRotation(NewLocation, NewRotation);

		if (!CurrentKeptIngredient)
		{
			return;
		}

		// 조리 완료가 지연 처리되는 재료의 경우 도마 등에 올려놨을 때 조리를 완료해 다음 재료로 데이터 변경
		// ex) 쌀가루 찜기의 경우 가스레인지에서 조리가 끝나면 들어서 도마나 접시 등에 올려놔야 쌀반죽, 백설기 등으로 변경됨
		if (bIsPendingCookingCompletable)
		{
			CurrentKeptIngredient->CompletePendingCooking(ECookingTool::Put);
		}

		// Automatic일 경우 요리를 바로 시작하기 위해 HandleCook을 호출
		if (bIsAutomatic)
		{
			HandleCook();
		}
	}
	// Countertop이 보관 중인 액터가 있으면, 플레이어가 해당 액터를 잡도록 함
	else
	{
		if (Grabbed)
		{
			ADish* Dish = nullptr;
			AIngredient* Ingredient = nullptr;
			AActor* GrabbedActor = Grabbed->GetOwner();

			bool bIsKeptObjectDish = CurrentKeptObject->IsA<ADish>();
			bool bIsGrabbedObjectDish = GrabbedActor->IsA<ADish>();

			// 두 액터 모두 Dish이거나 Dish가 없을 경우 수행할 작업이 없으므로 return
			if (bIsKeptObjectDish == bIsGrabbedObjectDish)
			{
				return;
			}

			Dish = bIsKeptObjectDish ? Cast<ADish>(CurrentKeptObject) : Cast<ADish>(GrabbedActor);
			Ingredient = bIsKeptObjectDish ? Cast<AIngredient>(GrabbedActor) : Cast<AIngredient>(CurrentKeptObject);

			// Ingredient가 아닌 다른 액터일 경우 수행할 작업이 없으므로 return
			if (!Ingredient)
			{
				return;
			}

			bool bDishSetResult = Dish->SetDessert(Ingredient);
			if (!bDishSetResult)
			{
				return;
			}

			if (bIsKeptObjectDish)
			{
				Grabber->Release();
			}
			else
			{
				ResetCooking(true);
			}

			CookingStateIndicator->Hide();
			return;
		}

		UPrimitiveComponent* Primitive = CurrentKeptObject->GetComponentByClass<UPrimitiveComponent>();
		const FVector& GrabPoint = KeepPoint->GetComponentLocation();

		Primitive->SetSimulatePhysics(true);
		Grabber->Grab(Primitive, GrabPoint);

		CookingStateIndicator->Hide();
		ResetCooking(true);
	}
}
