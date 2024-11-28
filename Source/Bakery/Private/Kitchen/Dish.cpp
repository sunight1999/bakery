// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Dish.h"
#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

#include "Kitchen/KitchenDefines.h"
#include "Kitchen/Ingredient.h"
#include "Kitchen/IngredientContainer.h"
#include "Kitchen/Data/IngredientData.h"
#include "Interactions/GrabberComponent.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Abnormality/Components/FlyAwayComponent.h"

ADish::ADish()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionBox->SetSimulatePhysics(true);
	InteractionBox->SetEnableGravity(false);

	DishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DishMesh"));
	DishMesh->SetupAttachment(RootComponent);

	// Collision 설정
	DishMesh->SetMobility(EComponentMobility::Movable);
	DishMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	DishMesh->SetCollisionProfileName(FName("Ingredient"));

	// Physics 설정
	DishMesh->SetSimulatePhysics(false);
	DishMesh->SetEnableGravity(false);

	FlyAwayEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlyAwayEffect"));
	FlyAwayEffect->SetAutoActivate(false);
	FlyAwayEffect->SetupAttachment(RootComponent);
}

void ADish::BeginPlay()
{
	Super::BeginPlay();

	InteractionBox->SetBoxExtent(FVector::ZeroVector, false);
}

void ADish::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	if (Grabbed || !bIsFlying)
	{
		return;
	}

	Caught();

	UPrimitiveComponent* Primitive = GetComponentByClass<UPrimitiveComponent>();
	Grabber->Grab(Primitive, GetActorLocation());
}

void ADish::OnEnterHighlight()
{
	Super::OnEnterHighlight();

	if (!HighlightOverlayMaterial || !Dessert)
	{
		return;
	}

	TArray<UStaticMeshComponent*> MeshComponents;
	Dessert->GetComponents(MeshComponents);

	for (auto MeshComponent : MeshComponents)
	{
		MeshComponent->SetOverlayMaterial(HighlightOverlayMaterial);
	}
}

void ADish::OnExitHighlight()
{
	Super::OnExitHighlight();

	if (!HighlightOverlayMaterial || !Dessert)
	{
		return;
	}

	TArray<UStaticMeshComponent*> MeshComponents;
	Dessert->GetComponents(MeshComponents);

	for (auto MeshComponent : MeshComponents)
	{
		MeshComponent->SetOverlayMaterial(nullptr);
	}
}

bool ADish::IsSettable(AIngredient* Ingredient)
{
	return Ingredient->GetSourceRecipe() && !Dessert;
}

bool ADish::SetDessert(AIngredient* Ingredient)
{
	Ingredient->CompletePendingCooking(ECookingTool::Dish);

	if (!IsSettable(Ingredient))
	{
		return false;
	}

	Dessert = Ingredient;

	UPrimitiveComponent* Primitive = Dessert->GetComponentByClass<UPrimitiveComponent>();
	Primitive->SetSimulatePhysics(false);
	Primitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Primitive->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	Dessert->SetActorLocation(GetActorLocation() + FVector::UpVector * DishThickness);
	Dessert->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	bIsClean = false;

	return true;
}

AIngredient* ADish::PeekDessert()
{
	return Dessert;
}

void ADish::Eat()
{
	// TODO: 재료 풀링을 각 재료 보관함이 아니라 하나의 오브젝트가 수행하도록 수정
	bool bIsReturned = false;
	for (TActorIterator<AIngredientContainer> It(GetWorld()); It; ++It)
	{
		bIsReturned = (*It)->TryReturnIngredient(Dessert);
		if (bIsReturned)
		{
			break;
		}
	}

	if (!bIsReturned)
	{
		UE_LOG(LogTemp, Display, TEXT("%s의 적절한 재료 보관함을 찾을 수 없습니다."), *Dessert->GetActorNameOrLabel());
		Dessert->Destroy();
	}

	Dessert = nullptr;
}

void ADish::Wash()
{
	if (bIsClean)
	{
		return;
	}

	if (++CurrentWashingTime == HandWashingTime)
	{
		bIsClean = true;
		CurrentWashingTime = 0;
	}
}

void ADish::FlyAway(float InMoveSpeed, float InFlySpeed, float InAmplitude, float InMoveInterval)
{
	bIsFlying = true;
	bIsLostDish = true;

	UActorComponent* ActorComponent = AddComponentByClass(UFlyAwayComponent::StaticClass(), false, FTransform::Identity, false);
	UFlyAwayComponent* FlyAwayComponent = Cast<UFlyAwayComponent>(ActorComponent);
	FlyAwayComponent->SetFlyOptions(InMoveSpeed, InFlySpeed, InAmplitude, InMoveInterval);

	InteractionBox->SetBoxExtent(OriginInteractionBoxExtent, false);
	FlyAwayEffect->Activate();
}

void ADish::Caught()
{
	bIsFlying = false;

	UFlyAwayComponent* FlyAwayComponent = GetComponentByClass<UFlyAwayComponent>();
	FlyAwayComponent->DestroyComponent();

	InteractionBox->SetBoxExtent(FVector::ZeroVector, false);
	FlyAwayEffect->Deactivate();
}
