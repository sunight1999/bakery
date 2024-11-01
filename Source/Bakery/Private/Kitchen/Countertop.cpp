// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Countertop.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Interactions/Interactables/InteractableComponent.h"

#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/Data/IngredientData.h"
#include "Subsystems/RecipeSubsystem.h"
#include "Characters/PlayerPawn.h"
#include "Subsystems/SoundManager.h"
#include "Components/AudioComponent.h"

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
}

void ACountertop::Tick(float DeltaTime)
{
	if (bIsCooking && bIsAutomatic)
	{
		CurrentAutoCookingTime += DeltaTime;
		UE_LOG(LogTemp, Display, TEXT("( %f / %f )"), CurrentAutoCookingTime, AutoCookingTime);

		if (IsCookingDone())
		{
			EndCook();
		}
	}
}

void ACountertop::Reset()
{
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
	PrimaryCookingEffect->Deactivate();

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

	// TODO: 요리 진행바 UI 띄우기

	// 요리 이펙트 재생
	PrimaryCookingEffect->Activate();

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
		UE_LOG(LogTemp, Display, TEXT("( %d / %d )"), CurrentHandCookingTime, HandCookingTime);
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

	if (InteractionInfo.Interactor)
	{
		Grabber = InteractionInfo.Interactor->GetGrabber();
		Grabbed = Grabber->GetGrabbed();

		if (Grabbed)
		{
			GrabbedIngredient = Cast<AIngredient>(Grabbed->GetOwner());
		}
	}

	// 플레이어가 잡고 있는 재료가 있을 경우 재료 합치기 시도
	if (GrabbedIngredient && CurrentKeptIngredient)
	{
		const UIngredientData* MergedIngredientData = CurrentKeptIngredient->MergeIngredient(GrabbedIngredient);
		if (MergedIngredientData)
		{
			Grabber->Release();
			GrabbedIngredient->Destroy();
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

void ACountertop::OnInteract()
{
}

void ACountertop::OnExitInteract()
{
}

/*
 * Grab Interaction Functions
 */
void ACountertop::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	// 현재 Countertop이 보관 중인 액터가 없으면, 플레이어의 Grabber가 쥐고 있는 액터를 가져와 KeepPoint에 올림
	if (!CurrentKeptIngredient)
	{
		// Grabber에 액터가 없다면 더 이상 수행할 작업이 없으므로 종료
		if (!Grabbed)
		{
			return;
		}

		AIngredient* GrabbedIngredient = Cast<AIngredient>(Grabbed->GetOwner());
		if (!GrabbedIngredient)
		{
			return;
		}

		FVector NewLocation = KeepPoint->GetComponentLocation();
		FRotator NewRotation = GrabbedIngredient->GetActorRotation();
		NewRotation.Pitch = 0.f;

		Grabber->Release();

		// 충돌 오류 방지를 위해 Collision과 Physics 비활성화
		Grabbed->SetSimulatePhysics(false);
		GrabbedIngredient->SetActorLocationAndRotation(NewLocation, NewRotation);

		CurrentKeptIngredient = GrabbedIngredient;

		// Automatic일 경우 요리를 바로 시작하기 위해 OnEnterInteract를 호출
		if (bIsAutomatic)
		{
			OnEnterInteract(FInteractionInfo::NoneInteraction);
		}
	}
	// Countertop이 보관 중인 재료가 있으면, 플레이어가 해당 재료를 잡도록 함
	else
	{
		if (Grabbed)
		{
			return;
		}

		UPrimitiveComponent* Primitive = CurrentKeptIngredient->GetComponentByClass<UPrimitiveComponent>();
		const FVector& GrabPoint = KeepPoint->GetComponentLocation();

		Primitive->SetSimulatePhysics(true);
		Grabber->Grab(Primitive, GrabPoint);

		Reset();
	}
}

void ACountertop::OnGrab()
{
	UE_LOG(LogTemp, Display, TEXT("GRABBING"));
}

void ACountertop::OnExitGrab()
{
	UE_LOG(LogTemp, Display, TEXT("GRAB EXIT"));
}
