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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta=(AllowPrivateAccess = "true"))
    int32 decodeCipherCount = 0;

public:
    void AddDecodeCipher();

    int32 GetDecodeCipherCount();

private:
    // url, sheetName, 원하는 스트럭처를 넣어주면 알아서 해당 스트럭처 리스트에 시트 값을 넣어서 반환해준다.
    /*template <typename T> ReadCSVData(FString url, FString sheetName, T bindingStruct, FString range = "");*/

    FString ReadCSVData(FString url, FString sheetName, FString range = "");

    void ReadSurvivorData();
};