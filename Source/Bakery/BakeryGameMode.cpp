// Copyright Epic Games, Inc. All Rights Reserved.

#include "BakeryGameMode.h"
#include "BakeryCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABakeryGameMode::ABakeryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
