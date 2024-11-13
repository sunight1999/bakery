// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define BAKERY_SAVEGAME_SLOT_NAME FString("Bakery")

UENUM(BlueprintType)
enum class EBakeryState : uint8
{
	Closed,
	Opened
};

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Left,
	Right,
	Forward,
	Backward
};

UENUM(BlueprintType)
enum class ECustomerState : uint8
{
	Idle,
	Sitting,
	Ordering,
	WaitingDish,
	Eating,
	Standing,
	Leaving
};