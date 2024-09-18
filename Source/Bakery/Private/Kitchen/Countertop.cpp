// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Countertop.h"
#include "Components/BoxComponent.h"

#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Interactions/Interactables/InteractableComponent.h"
#include "Characters/PlayerPawn.h"

ACountertop::ACountertop()
{
	BottomSupportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomSupportMesh"));
	BottomSupportMesh->SetupAttachment(InteractionBox);

	TopPlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopPlateMesh"));
	TopPlateMesh->SetupAttachment(BottomSupportMesh);

	OptionalToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OptionalToolMesh"));
	OptionalToolMesh->SetupAttachment(TopPlateMesh);

	KeepPoint = CreateDefaultSubobject<USceneComponent>(TEXT("KeepPoint"));
	KeepPoint->SetupAttachment(TopPlateMesh);
}

void ACountertop::BeginPlay()
{
}

void ACountertop::Tick(float DeltaTime)
{
}

/*
 * Interaction Functions
 */
void ACountertop::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	UE_LOG(LogTemp, Display, TEXT("INTERACT ENTER"));
}

void ACountertop::OnInteract()
{
	UE_LOG(LogTemp, Display, TEXT("INTERACTING"));
}

void ACountertop::OnExitInteract()
{
	UE_LOG(LogTemp, Display, TEXT("ITNERACT EXIT"));
}

/*
 * Grab Interaction Functions
 */
void ACountertop::OnEnterGrab(const FInteractionInfo& InteractionInfo)
{
	UGrabberComponent* Grabber = InteractionInfo.Interactor->GetGrabber();
	UPrimitiveComponent* Grabbed = Grabber->GetGrabbed();

	// 현재 Countertop이 보관 중인 액터가 없으면, 플레이어의 Grabber가 쥐고 있는 액터를 가져와 KeepPoint에 올림
	if (!CurrentKeptActor)
	{
		// Grabber에 액터가 없다면 더 이상 수행할 작업이 없으므로 종료
		if (!Grabbed)
		{
			return;
		}

		AActor* GrabbedActor = Grabbed->GetOwner();
		FVector NewLocation = KeepPoint->GetComponentLocation();
		FRotator NewRotation = GrabbedActor->GetActorRotation();
		NewRotation.Pitch = 0.f;

		Grabber->Release();

		// 충돌 오류 방지를 위해 Collision과 Physics 비활성화
		GrabbedActor->SetActorEnableCollision(false);
		Grabbed->SetSimulatePhysics(false);
		GrabbedActor->SetActorLocationAndRotation(NewLocation, NewRotation);

		CurrentKeptActor = GrabbedActor;
	}
	// Countertop이 보관 중인 액터가 있으면, 플레이어의 Grabber가 해당 액터를 잡도록 함
	else
	{
		// Grabber에 액터가 있다면 Countertop이 이미 보관 중인 액터가 있어 수행할 작업이 없으므로 종료
		if (Grabbed)
		{
			return;
		}

		UPrimitiveComponent* Primitive = CurrentKeptActor->GetComponentByClass<UPrimitiveComponent>();
		const FVector& GrabPoint = KeepPoint->GetComponentLocation();

		// 비활성화했던 Collision과 Physics를 활성화
		CurrentKeptActor->SetActorEnableCollision(true);
		Primitive->SetSimulatePhysics(true);

		Grabber->Grab(Primitive, GrabPoint);
		CurrentKeptActor = nullptr;
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
