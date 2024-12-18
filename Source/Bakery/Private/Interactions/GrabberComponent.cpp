// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/GrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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

void UGrabberComponent::Grab(UPrimitiveComponent* Primitive, const FVector& ImpactPoint)
{
	Primitive->WakeAllRigidBodies();

	// 잡고 있는 오브젝트가 Interactor의 트레이스에 걸리지 않도록 Collision 설정 변경
	Primitive->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Primitive->SetSimulatePhysics(true);

	PhysicsHandle->GrabComponentAtLocationWithRotation(
		Primitive,
		NAME_None,
		ImpactPoint,
		GrabPoint->GetComponentRotation()
	);

	// 잡기 효과음 재생
	USoundManager::GetInstance(GetWorld())->PlaySoundAtLocationByTag(FName("PickUp"), GetComponentLocation());

	OnGrab.Broadcast();
}

void UGrabberComponent::ReleaseBegin()
{
}

void UGrabberComponent::Release()
{
	UPrimitiveComponent* Grabbed = PhysicsHandle->GetGrabbedComponent();
	if (!Grabbed)
	{
		return;
	}

	Grabbed->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PhysicsHandle->ReleaseComponent();

	// 놓기 효과음 재생
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

