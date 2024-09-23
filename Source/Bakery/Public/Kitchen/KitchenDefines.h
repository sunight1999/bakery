// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define KITCHEN_ASSETTYPE_RECIPE FPrimaryAssetType("Recipe")
#define KITCHEN_ASSETTYPE_INGREDIENT FPrimaryAssetType("Ingredient")

UENUM(BlueprintType)
enum class ECookingTool : uint8
{
	Knife,
	PatternMold,
	Oven,
	Steamer,
};