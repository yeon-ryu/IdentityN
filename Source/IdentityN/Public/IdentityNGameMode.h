// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IdentityNGameMode.generated.h"

UCLASS(minimalapi)
class AIdentityNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIdentityNGameMode();

    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SurvivorGameData)
    TArray<class ASurvivor*> escapeList;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SurvivorGameData)
    TArray<class ASurvivor*> eliminateList;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta=(AllowPrivateAccess = "true"))
    int32 decodeCipherCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay, meta=(AllowPrivateAccess = "true"))
    int32 escapeCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay, meta=(AllowPrivateAccess = "true"))
    int32 eliminateCount = 0;

public:
    void AddDecodeCipher();

    int32 GetDecodeCipherCount();

    void Escape(class ASurvivor* sur);

    void Eliminate(class ASurvivor* sur);

    void GameEnd();
};