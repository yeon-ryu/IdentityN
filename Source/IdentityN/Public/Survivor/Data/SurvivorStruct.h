// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivorStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSurvivorData {
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    int32 id;

    UPROPERTY(BlueprintReadWrite)
    FString name;

    UPROPERTY(BlueprintReadWrite)
    float runSpeed;

    UPROPERTY(BlueprintReadWrite)
    float crouchSpeed;

    UPROPERTY(BlueprintReadWrite)
    float crawlSpeed;
};

/**
 * 
 */
class IDENTITYN_API SurvivorStruct
{
public:
	SurvivorStruct();
	~SurvivorStruct();
};