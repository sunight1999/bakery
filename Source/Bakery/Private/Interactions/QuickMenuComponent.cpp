// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/QuickMenuComponent.h"

UQuickMenuComponent::UQuickMenuComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UQuickMenuComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UQuickMenuComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

