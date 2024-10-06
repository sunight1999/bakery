// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EBakeryState : uint8
{
	Preparing,
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
	Eating
};