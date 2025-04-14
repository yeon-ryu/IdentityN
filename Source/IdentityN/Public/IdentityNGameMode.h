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

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta=(AllowPrivateAccess = "true"))
    int32 decodeCipherCount = 0;

public:
    void AddDecodeCipher();

    int32 GetDecodeCipherCount();
};