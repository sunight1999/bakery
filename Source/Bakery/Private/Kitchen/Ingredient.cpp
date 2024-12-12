// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Ingredient.h"
#include "UObject/ObjectSaveContext.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

#include "Subsystems/RecipeSubsystem.h"
#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Data/IngredientData.h"
#include "Kitchen/Data/IngredientMeshData.h"
#include "Kitchen/Data/RecipeData.h"
#include "General/BakeryGameState.h"
#include "General/Tools/DessertSceneCapturer.h"
#include "Subsystems/UISubsystem.h"
#include "Characters/PlayerPawn.h"
#include "Widgets/Bakery/DessertExplainWidget.h"

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = false;

	RootBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBoxComponent"));
	RootComponent = RootBoxComponent;

	// Collision 설정
	RootBoxComponent->SetMobility(EComponentMobility::Movable);
	RootBoxComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootBoxComponent->SetCollisionProfileName(FName("Ingredient"));

	// Physics 설정
	RootBoxComponent->SetSimulatePhysics(true);
	RootBoxComponent->SetEnableGravity(false);

	CookingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CookingEffect"));
	CookingEffect->SetAutoActivate(false);
	CookingEffect->SetupAttachment(RootComponent);

	/*
	 * StaticMeshComponent 설정
	 */
	IngredientBottomStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientBottomStaticMesh"));
	InitializeMeshComponent(IngredientBottomStaticMesh);

	IngredientBodyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientBodyStaticMesh"));
	InitializeMeshComponent(IngredientBodyStaticMesh);

	IngredientLidStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientLidStaticMesh"));
	InitializeMeshComponent(IngredientLidStaticMesh);

	/*
	 * SkeletalMeshComponent 설정
	 */
	IngredientBottomSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("IngredientBottomSkeletalMesh"));
	InitializeMeshComponent(IngredientBottomSkeletalMesh);

	IngredientBodySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("IngredientBodySkeletalMesh"));
	InitializeMeshComponent(IngredientBodySkeletalMesh);

	IngredientLidSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("IngredientLidSkeletalMesh"));
	InitializeMeshComponent(IngredientLidSkeletalMesh);
}

void AIngredient::InitializeMeshComponent(UMeshComponent* MeshComponent)
{
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AIngredient::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	RecipeSubsystem = GameInstance->GetSubsystem<URecipeSubsystem>();

	verify(RecipeSubsystem);
}

void AIngredient::PlayBeingCookedAnimation(ECookingTool CookingTool)
{
	const UIngredientMeshData* IngredientMeshData = IngredientData->GetIngredientMeshData();
	if (!IngredientMeshData->IsUsingStaticMesh())
	{
		IngredientBottomSkeletalMesh->PlayAnimation(IngredientMeshData->GetBottomSkeletalAnimation(), true);
		IngredientBodySkeletalMesh->PlayAnimation(IngredientMeshData->GetBodySkeletalAnimation(), true);
		IngredientLidSkeletalMesh->PlayAnimation(IngredientMeshData->GetLidSkeletalAnimation(), true);
	}

	if (UNiagaraSystem* Effect = IngredientData->GetCookingEffect(CookingTool))
	{
		const FVector& Offset = IngredientData->GetCookingEffectOffset(CookingTool);
		CookingEffect->SetRelativeLocation(Offset);
		CookingEffect->SetAsset(Effect);
		CookingEffect->Activate();
	}
}

void AIngredient::StopBeingCookedAnimation()
{
	const UIngredientMeshData* IngredientMeshData = IngredientData->GetIngredientMeshData();
	if (!IngredientMeshData->IsUsingStaticMesh())
	{
		IngredientBottomSkeletalMesh->Stop();
		IngredientBodySkeletalMesh->Stop();
		IngredientLidSkeletalMesh->Stop();
	}
	
	if (CookingEffect->IsActive())
	{
		CookingEffect->Deactivate();
		CookingEffect->SetAsset(nullptr);
	}
}

/// <summary>
/// 다른 재료로 데이터를 변경한다.
/// 변경된 재료가 레시피에 등록되어 있을 경우 해당 레시피 정보를 가져온다.
/// </summary>
/// <param name="TargetIngredientData">변경할 재료</param>
void AIngredient::ChangeIngredient(const UIngredientData* NewIngredientData, bool bIsRaw)
{
	if (!NewIngredientData)
	{
		return;
	}

	Clear();

	IngredientData = NewIngredientData;
	SourceRecipe = RecipeSubsystem->GetRecipe(NewIngredientData->GetName());

	SetIngredientMesh();

	CheckFirstCook();
}

/// <summary>
/// 재료를 다른 재료와 합칠 수 있는지 확인한다.
/// </summary>
/// <param name="OtherIngredient">합칠 재료</param>
/// <returns>두 재료를 사용하는 레시피의 결과물</returns>
const UIngredientData* AIngredient::TryMergeIngredient(const AIngredient* OtherIngredient)
{
	return TryMergeIngredient(OtherIngredient->GetIngredientData());
}

const UIngredientData* AIngredient::TryMergeIngredient(const UIngredientData* OtherIngredient)
{
	return IngredientData->CheckMergeable(OtherIngredient);
}

void AIngredient::CompletePendingCooking(ECookingTool CookingTool)
{
	if (!IngredientData->IsPendingCooking() || !bIsCooked)
	{
		return;
	}

	if (CookingTool != ECookingTool::Put && CookingTool != ECookingTool::Dish)
	{
		return;
	}

	const UIngredientData* CookedIngredientData = IngredientData->CheckCookable(CookingTool);
	ChangeIngredient(CookedIngredientData);
}

void AIngredient::CheckFirstCook()
{
	ABakeryGameState* BakeryGameState = Cast<ABakeryGameState>(GetWorld()->GetGameState());
	const FName& RecipeName = IngredientData->GetName();

	if (SourceRecipe && !BakeryGameState->IsCookedRecipe(RecipeName))
	{
		// 디저트 2D 씬 캡쳐 메시 설정
		if (!DessertSceneCapturer)
		{
			for (TActorIterator<ADessertSceneCapturer>It(GetWorld()); It; ++It)
			{
				DessertSceneCapturer = *It;
				break;
			}
		}

		const UIngredientMeshData* IngredientMeshData = IngredientData->GetIngredientMeshData();
		DessertSceneCapturer->SetDessertMesh(IngredientMeshData->GetBottomStaticMesh(), IngredientMeshData->GetBodyStaticMesh(), IngredientMeshData->GetLidStaticMesh());

		// 디저트 설명 UI 가시화
		UUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
		UUserWidget* UserWidget = UISubsystem->SetUIVisibility(FName("DessertExplain"), ESlateVisibility::SelfHitTestInvisible);
		UDessertExplainWidget* DessertExplainWidget = Cast<UDessertExplainWidget>(UserWidget);
		DessertExplainWidget->SetDessert(SourceRecipe);

		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		PlayerPawn->SetUIOpened(true);

		BakeryGameState->SetCookedRecipe(RecipeName);
	}
}

void AIngredient::Clear()
{
	IngredientBottomStaticMesh->SetStaticMesh(nullptr);
	IngredientBodyStaticMesh->SetStaticMesh(nullptr);
	IngredientLidStaticMesh->SetStaticMesh(nullptr);

	IngredientBottomSkeletalMesh->SetSkeletalMesh(nullptr);
	IngredientBodySkeletalMesh->SetSkeletalMesh(nullptr);
	IngredientLidSkeletalMesh->SetSkeletalMesh(nullptr);

	IngredientBottomSkeletalMesh->SetAnimation(nullptr);
	IngredientBodySkeletalMesh->SetAnimation(nullptr);
	IngredientLidSkeletalMesh->SetAnimation(nullptr);

	bIsCooked = false;
	IngredientData = nullptr;
	SourceRecipe = nullptr;
}

void AIngredient::SetIngredientMesh()
{
	const UIngredientMeshData* IngredientMeshData = IngredientData->GetIngredientMeshData();
	if (IngredientMeshData->IsUsingStaticMesh())
	{
		IngredientBottomStaticMesh->SetStaticMesh(IngredientMeshData->GetBottomStaticMesh());
		IngredientBodyStaticMesh->SetStaticMesh(IngredientMeshData->GetBodyStaticMesh());
		IngredientLidStaticMesh->SetStaticMesh(IngredientMeshData->GetLidStaticMesh());
	}
	else
	{
		IngredientBottomSkeletalMesh->SetSkeletalMesh(IngredientMeshData->GetBottomSkeletalMesh());
		IngredientBodySkeletalMesh->SetSkeletalMesh(IngredientMeshData->GetBodySkeletalMesh());
		IngredientLidSkeletalMesh->SetSkeletalMesh(IngredientMeshData->GetLidSkeletalMesh());
	}
}
