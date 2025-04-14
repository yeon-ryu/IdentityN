// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SurvivorEnum.h"
#include "SurvivorStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSurvivorData : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FString RowName;

    UPROPERTY(BlueprintReadWrite)
    int32 Id;

    UPROPERTY(BlueprintReadWrite)
    FString Name;

    UPROPERTY(BlueprintReadWrite)
    float RunSpeed;

    UPROPERTY(BlueprintReadWrite)
    float CrouchSpeed;

    UPROPERTY(BlueprintReadWrite)
    float CrawlSpeed;

    UPROPERTY(BlueprintReadWrite)
    float DecodeTime;

    UPROPERTY(BlueprintReadWrite)
    float DecodeFailPer;

    UPROPERTY(BlueprintReadWrite)
    float DecodeFailTime;

    UPROPERTY(BlueprintReadWrite)
    float OpenDoorTime;

    UPROPERTY(BlueprintReadWrite)
    float PullTime;

    UPROPERTY(BlueprintReadWrite)
    float PassBoardFast;

    UPROPERTY(BlueprintReadWrite)
    float PassBoardNormal;

    UPROPERTY(BlueprintReadWrite)
    float PassBoardSlow;

    UPROPERTY(BlueprintReadWrite)
    float PassWindowFast;

    UPROPERTY(BlueprintReadWrite)
    float PassWindowSlow;

    UPROPERTY(BlueprintReadWrite)
    float HealTime;

    UPROPERTY(BlueprintReadWrite)
    float TakeHealTime;

    UPROPERTY(BlueprintReadWrite)
    float HealSelfTime;

    UPROPERTY(BlueprintReadWrite)
    float RemainStepTime;

    UPROPERTY(BlueprintReadWrite)
    float DamageSpeedTime;

    UPROPERTY(BlueprintReadWrite)
    float BalloonedFallTime;

    UPROPERTY(BlueprintReadWrite)
    float SeatTakeOffTime;

    UPROPERTY(BlueprintReadWrite)
    float SeatEscapeTime;
};

USTRUCT(Atomic, BlueprintType)
struct FSurvivorPersona : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FString RowName;

    UPROPERTY(BlueprintReadWrite)
    int32 Id;

    UPROPERTY(BlueprintReadWrite)
    FString Name;

    UPROPERTY(BlueprintReadWrite)
    FString Desc;

    UPROPERTY(BlueprintReadWrite)
    ESurvivorPersonaTrigger Trigger;

    UPROPERTY(BlueprintReadWrite)
    FString Data; 
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