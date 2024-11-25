// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Countertop.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"

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

	CookingProgressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CookingProgressWidget"));
	CookingProgressWidget->SetupAttachment(InteractionBox);
	CookingProgressWidget->SetVisibility(false);

	PrimaryCookingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PrimaryCookingEffect"));
	PrimaryCookingEffect->SetAutoActivate(false);
	PrimaryCookingEffect->SetupAttachment(InteractionBox);

	SecondaryCookingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SecondaryCookingEffect"));
	SecondaryCookingEffect->SetAutoActivate(false);
	SecondaryCookingEffect->SetupAttachment(InteractionBox);
}

void ACountertop::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	RecipeSubsystem = GameInstance->GetSubsystem<URecipeSubsystem>();

	verify(RecipeSubsystem);

	CookingProgress = Cast<UProgressWidget>(CookingProgressWidget->GetUserWidgetObject());
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

void ACountertop::Reset()
{
	CurrentKeptObject = nullptr;
	CurrentKeptIngredient = nullptr;

	ResetCooking();
}

void ACountertop::ResetCooking()
{
	bIsCooking = false;
	CurrentAutoCookingTime = 0.f;
	CurrentHandCookingTime = 0;
	CurrentCookingTool = ECookingTool::None;
	CurrentCookingTarget = nullptr;

	CookingProgressWidget->SetVisibility(false);
	CookingProgress->SetPercentage(0.f);

	if (PrimaryCookingEffect)
	{
		PrimaryCookingEffect->Deactivate();
	}

	if (SecondaryCookingEffect)
	{
		SecondaryCookingEffect->Deactivate();
	}

	if (CurrentAudio)
	{
		CurrentAudio->Stop();
		CurrentAudio = nullptr;
	}
}

/*
 * 요리 관련 함수
 */
void ACountertop::BeginCook(ECookingTool CookingTool, const UIngredientData* CookingTarget)
{
	verify(CookingTarget);

	CurrentCookingTool = CookingTool;
	CurrentCookingTarget = CookingTarget;

	CookingProgressWidget->SetVisibility(true);

	// 요리 이펙트 재생
	if (PrimaryCookingEffect)
	{
		PrimaryCookingEffect->Activate();
	}

	if (SecondaryCookingEffect)
	{
		SecondaryCookingEffect->Activate();
	}

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
	}

	if (IsCookingDone())
	{
		EndCook();
	}
}

void ACountertop::EndCook()
{
	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("CookingDone"), GetActorLocation());

	CurrentKeptIngredient->ChangeIngredient(CurrentCookingTarget);

	ResetCooking();

	// 더 이상 요리가 불가능 할 때까지 자동으로 요리 진행
	if (bIsAutomatic)
	{
		OnEnterInteract(FInteractionInfo::NoneInteraction);
	}
}

/*
 * Interaction Functions
 */
void ACountertop::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = nullptr;
	UPrimitiveComponent* Grabbed = nullptr;
	AIngredient* GrabbedIngredient = nullptr;

	// FInteractionInfo::NoneInteraction를 사용하는 경우가 있어 Interactor 유효성 검사 필요
	if (InteractionInfo.Interactor)
	{
		Grabber = InteractionInfo.Interactor->GetGrabber();
		Grabbed = Grabber->GetGrabbed();

		if (Grabbed)
		{
			GrabbedIngredient = Cast<AIngredient>(Grabbed->GetOwner());
		}
	}

	// 플레이어가 잡고 있는 액터가 재료일 경우 재료 합치기 시도
	if (GrabbedIngredient && CurrentKeptIngredient)
	{
		const UIngredientData* MergedIngredientData = CurrentKeptIngredient->MergeIngredient(GrabbedIngredient);
		if (MergedIngredientData)
		{
			Grabber->Release();
			
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

	// 진행 중인 요리가 있다면 요리 진행
	if (bIsCooking)
	{
		Cook();
		return;
	}

	if (!CurrentKeptIngredient)
	{
		return;
	}

	// 현재 Countertop에서 사용 가능한 요리 도구 중 CurrentKeptIngredient와 맞는 게 있으면 요리 시작
	const UIngredientData* IngredientData = CurrentKeptIngredient->GetIngredientData();
	for (ECookingTool CookingTool : AvailableCookingTools)
	{
		if (const UIngredientData* CookingTarget = IngredientData->CheckCookable(CookingTool))
		{
			BeginCook(CookingTool, CookingTarget);
			return;
		}
	}
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

		CurrentKeptObject = Grabbed->GetOwner();
		CurrentKeptIngredient = Cast<AIngredient>(CurrentKeptObject);

		FVector NewLocation = KeepPoint->GetComponentLocation();
		FRotator NewRotation = CurrentKeptObject->GetActorRotation();
		NewRotation.Pitch = 0.f;

		Grabber->Release();

		// 충돌 오류 방지를 위해 Collision과 Physics 비활성화
		Grabbed->SetSimulatePhysics(false);
		CurrentKeptObject->SetActorLocationAndRotation(NewLocation, NewRotation);

		// Automatic일 경우 요리를 바로 시작하기 위해 OnEnterInteract를 호출
		if (CurrentKeptIngredient && bIsAutomatic)
		{
			OnEnterInteract(FInteractionInfo::NoneInteraction);
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
				Reset();
			}

			return;
		}

		UPrimitiveComponent* Primitive = CurrentKeptObject->GetComponentByClass<UPrimitiveComponent>();
		const FVector& GrabPoint = KeepPoint->GetComponentLocation();

		Primitive->SetSimulatePhysics(true);
		Grabber->Grab(Primitive, GrabPoint);

		Reset();
	}
}
