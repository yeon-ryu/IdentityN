// Copyright Epic Games, Inc. All Rights Reserved.

#include "IdentityNGameMode.h"
#include "IdentityNCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIdentityNGameMode::AIdentityNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
