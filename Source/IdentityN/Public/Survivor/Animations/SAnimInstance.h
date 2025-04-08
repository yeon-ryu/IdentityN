// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Data/SurvivorEnum.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IDENTITYN_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SurvivorAnim)
    ESurvivorState State = ESurvivorState::IDLE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SurvivorAnim)
	float Speed = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= SurvivorAnim)
	bool bCrouch = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= SurvivorAnim)
	bool bCrawl = false;
};
