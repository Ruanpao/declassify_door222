// Copyright Epic Games, Inc. All Rights Reserved.

#include "declassify_doorGameMode.h"
#include "declassify_doorCharacter.h"
#include "UObject/ConstructorHelpers.h"

Adeclassify_doorGameMode::Adeclassify_doorGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
