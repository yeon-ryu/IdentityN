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

void AIdentityNGameMode::BeginPlay()
{
    // 최초에 한번 CSV 에서 읽어와서 리스트나 맵에 넣어놓기
    ReadCSVData();
}

void AIdentityNGameMode::ReadCSVData()
{

}
