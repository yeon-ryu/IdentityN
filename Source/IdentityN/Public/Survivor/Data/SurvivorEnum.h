// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESurvivorState : uint8 {
    READY UMETA(DisplayName = "대기"), 
    IDLE UMETA(DisplayName = "기본상태"),
    //MOVE UMETA(DisplayName = "이동"),
    DAMAGED UMETA(DisplayName = "피격"),
    HEAL UMETA(DisplayName = "치료"),
    DECODE UMETA(DisplayName = "해독"),
    DECODE_FAIL UMETA(DisplayName = "해독실패"),
    OPEN UMETA(DisplayName = "문열기"),
    BALLOONED UMETA(DisplayName = "끌려감"), // UI 표시
    SEAT UMETA(DisplayName = "의자앉음"), // UI 표시
    RESCUE UMETA(DisplayName = "구출"),
    PASSBOARD UMETA(DisplayName = "판자넘어가는중"),
    PASSWINDOW UMETA(DisplayName = "창문넘어가는중"),
    PULLDOWN UMETA(DisplayName = "판자넘어뜨리는중"),
    SKILL UMETA(DisplayName = "스킬1"),
    SUCCESS UMETA(DisplayName = "탈출"), // UI 표시
    FAIL UMETA(DisplayName = "실종") // UI 표시
};

UENUM(BlueprintType)
enum class ESurvivorPersonaTrigger : uint8 {
    CompleteDecode UMETA(DisplayName = "해독완료"),
    Damaged UMETA(DisplayName = "피격 시")
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
