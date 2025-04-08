// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESurvivorState : uint8 {
    READY UMETA(DisplayName = "대기"), 
    IDLE UMETA(DisplayName = "정지"),
    RUN UMETA(DisplayName = "달리기"),
    DAMAGE UMETA(DisplayName = "피격"),
    HEAL UMETA(DisplayName = "치료"),
    DECODE UMETA(DisplayName = "해독"),
    DECODE_FAIL UMETA(DisplayName = "해독실패"),
    BALLOONED UMETA(DisplayName = "끌려감"),
    SIT UMETA(DisplayName = "의자앉음")
};

/**
 * 
 */
class IDENTITYN_API SurvivorEnum
{
public:
	SurvivorEnum();
	~SurvivorEnum();
};
