// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TPSPlayerAnim)
	float Speed = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=TPSPlayerAnim)
	bool bCrouch = false;
};
