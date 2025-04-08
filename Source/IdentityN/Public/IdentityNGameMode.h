// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Survivor/Data/SurvivorStruct.h"
#include "IdentityNGameMode.generated.h"

UCLASS(minimalapi)
class AIdentityNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIdentityNGameMode();

    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    TMap<int32, FSurvivorData> SurvivorDataMap;

private:
    void ReadCSVData();
};



