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

void AIngredientContainer::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	verify(ContainedIngredient);
}

/*
 * Interaction Functions
 */
void AIngredientContainer::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
}

void AIngredientContainer::OnInteract()
{
}

void AIngredientContainer::OnExitInteract()
{
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

	// Ingredient 액터를 생성해 플레이어에게 Grab
	// TODO: 풀링으로 변경
	AIngredient* Ingredient = World->SpawnActor<AIngredient>();
	Ingredient->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	Ingredient->SetActorLocationAndRotation(GetActorLocation(), FRotator::ZeroRotator);
	Ingredient->ChangeIngredient(ContainedIngredient.Get());

	UPrimitiveComponent* Primitive = Ingredient->GetComponentByClass<UPrimitiveComponent>();
	Grabber->Grab(Primitive, Ingredient->GetActorLocation());
}

void AIngredientContainer::OnGrab()
{
}

void AIngredientContainer::OnExitGrab()
{
}


