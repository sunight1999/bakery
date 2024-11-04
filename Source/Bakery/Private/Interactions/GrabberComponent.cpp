// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/GrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Interactions/Interactables/InteractableComponent.h"
#include "Subsystems/SoundManager.h"

UGrabberComponent::UGrabberComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
}

void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	GrabPoint->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 쥐고 있는 오브젝트 위치 처리
	if (PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GrabPoint->GetComponentLocation();
		FRotator TargetRotation = GrabPoint->GetComponentRotation();

		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, TargetRotation);
	}
}

void UGrabberComponent::Grab(FHitResult HitResult)
{
	UPrimitiveComponent* Primitive = HitResult.GetComponent();
	const FVector& ImpactPoint = HitResult.ImpactPoint;

	Grab(Primitive, ImpactPoint);
}

void UGrabberComponent::Grab(UPrimitiveComponent* Primitive, const FVector& ImpactPoint)
{
	Primitive->WakeAllRigidBodies();

	// 잡고 있는 오브젝트가 Interactor의 트레이스에 걸리지 않도록 Collision 설정 변경
	Primitive->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	PhysicsHandle->GrabComponentAtLocationWithRotation(
		Primitive,
		NAME_None,
		ImpactPoint,
		GrabPoint->GetComponentRotation()
	);

	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("PickUp"), GetComponentLocation());
	OnGrab.Broadcast();
}

void UGrabberComponent::ReleaseBegin()
{
	// 놓기 버튼 누르고 있는 시간 측정 시작
}

void UGrabberComponent::Release()
{
	UPrimitiveComponent* Grabbed = PhysicsHandle->GetGrabbedComponent();
	if (!Grabbed)
	{
		return;
	}

	AActor* GrabbedOwner = Grabbed->GetOwner();
	UInteractableComponent* Interactable = GrabbedOwner->GetComponentByClass<UInteractableComponent>();

	Grabbed->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	/*
	if (Interactable)
	{
		// 놓기 버튼 기능 구현을 IReleasable 인터페이스 만들어서 Interactable이 구현하게 변경하기

		if (Interactable->ComponentHasTag(INTERACTABLE_COMPONENT_GRABBALE))
		{
			// 놓기 버튼 누른 시간에 따라 강도 조절

			PhysicsHandle->ReleaseComponent();
			return;
		}

		if (Interactable->ComponentHasTag(INTERACTABLE_COMPONENT_REMODELABLE))
		{
			// 해당 위치에 오브젝트 설치

			PhysicsHandle->ReleaseComponent();
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractableComponent가 존재하지 않는 Actor(%s)를 잡고 있습니다."), *GrabbedOwner->GetActorNameOrLabel());
	}
	*/

	PhysicsHandle->ReleaseComponent();

	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("Put"), GetComponentLocation());
	OnRelease.Broadcast();

	return;
}

bool UGrabberComponent::IsGrabbing()
{
	return PhysicsHandle->GetGrabbedComponent() != nullptr;
}

UPrimitiveComponent* UGrabberComponent::GetGrabbed()
{
	return PhysicsHandle->GetGrabbedComponent();
}

