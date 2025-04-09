// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESurvivorState : uint8 {
    READY UMETA(DisplayName = "대기"), 
    IDLE UMETA(DisplayName = "정지"),
    MOVE UMETA(DisplayName = "이동"),
    DAMAGED UMETA(DisplayName = "피격"),
    HEAL UMETA(DisplayName = "치료"),
    DECODE UMETA(DisplayName = "해독"),
    DECODE_FAIL UMETA(DisplayName = "해독실패"),
    BALLOONED UMETA(DisplayName = "끌려감"), // UI 표시
    SIT UMETA(DisplayName = "의자앉음"), // UI 표시
    RESCUE UMETA(DisplayName = "구출"),
    PASS UMETA(DisplayName = "넘어가는중"),
    PULLDOWN UMETA(DisplayName = "판자넘어뜨리는중"),
    SKILL UMETA(DisplayName = "스킬1"),
    SUCCESS UMETA(DisplayName = "탈출"), // UI 표시
    FAIL UMETA(DisplayName = "실종") // UI 표시
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
