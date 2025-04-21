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

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SurvivorAnim)
    ESurvivorState State = ESurvivorState::IDLE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SurvivorAnim)
	float Speed = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= SurvivorAnim)
	bool bCrouch = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= SurvivorAnim)
	bool bCrawl = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= SurvivorAnim)
    bool bHurt = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= SurvivorAnim)
    bool falling = false;

private:
    class ASurvivor* survivor;

public:
    UFUNCTION()
    void AnimNotify_SDamageEnd();

    UFUNCTION()
    void AnimNotify_SHitFallEnd();
};
