// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/IngredientContainer.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Kitchen/Ingredient.h"

AIngredientContainer::AIngredientContainer()
{
	ContainerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	ContainerMesh->SetupAttachment(RootComponent);
}

void AIngredientContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIngredientContainer::End()
{
	IngredientPool.End();
}

void AIngredientContainer::BeginPlay()
{
	Super::BeginPlay();

	verify(ContainedIngredient);

	IngredientPool.OnPostSpawned.BindUObject(this, &AIngredientContainer::InitializeIngredient);
	IngredientPool.Initialize(GetWorld(), AIngredient::StaticClass(), this);
}

void AIngredientContainer::InitializeIngredient(UObject* IngredientObj)
{
	AIngredient* Ingredient = Cast<AIngredient>(IngredientObj);
	if (!Ingredient)
	{
		return;
	}

	Ingredient->SetActorLocationAndRotation(GetActorLocation(), FRotator::ZeroRotator);
	Ingredient->SetActorScale3D(FVector(IngredientMeshSizeMultiplier));
	Ingredient->ChangeIngredient(ContainedIngredient, true);

	UPrimitiveComponent* Primitive = Ingredient->GetComponentByClass<UPrimitiveComponent>();
	if (!Primitive)
	{
		return;
	}

	Primitive->SetConstraintMode(EDOFMode::XYPlane);
}

bool AIngredientContainer::TryReturnIngredient(UObject* IngredientObj)
{
	AIngredient* Ingredient = Cast<AIngredient>(IngredientObj);
	if (!Ingredient)
	{
		return false;
	}

	UPrimitiveComponent* Primitive = Ingredient->GetComponentByClass<UPrimitiveComponent>();
	Primitive->SetSimulatePhysics(true);

	return IngredientPool.Put(IngredientObj);
}

/*
 * Grab Interaction Functions
 */
void AIngredientContainer::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	// 이미 플레이어가 쥐고 있는 물건이 있을 경우 return
	if (Grabbed)
	{
		return;
	}

	// Ingredient 액터를 플레이어가 쥐도록 설정
	AIngredient* Ingredient = Cast<AIngredient>(IngredientPool.Get());
	Ingredient->ChangeIngredient(ContainedIngredient, true);

	UPrimitiveComponent* Primitive = Ingredient->GetComponentByClass<UPrimitiveComponent>();
	Grabber->Grab(Primitive, Ingredient->GetActorLocation());
}
