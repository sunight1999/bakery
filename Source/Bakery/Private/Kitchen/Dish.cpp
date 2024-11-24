// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Dish.h"
#include "EngineUtils.h"
#include "Components/BoxComponent.h"

#include "Kitchen/Ingredient.h"
#include "Kitchen/IngredientContainer.h"

ADish::ADish()
{
	PrimaryActorTick.bCanEverTick = false;

	DishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	RootComponent = DishMesh;

	// Collision 설정
	DishMesh->SetMobility(EComponentMobility::Movable);
	DishMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	DishMesh->SetCollisionProfileName(FName("Ingredient"));

	// Physics 설정
	DishMesh->SetSimulatePhysics(true);
	DishMesh->SetEnableGravity(false);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADish::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ADish::IsSettable(AIngredient* Ingredient)
{
	return Ingredient->GetAvailableRecipe() && !Dessert;
}

bool ADish::SetDessert(AIngredient* Ingredient)
{
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

