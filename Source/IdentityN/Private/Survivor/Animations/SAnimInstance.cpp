// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Animations/SAnimInstance.h"
#include "Survivor/Characters/Survivor.h"

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if(survivor == nullptr) {
        survivor = Cast<ASurvivor>(TryGetPawnOwner());

        if (survivor == nullptr) {
            return;
        }
    }

    FVector velocity = survivor->GetVelocity();
    FVector forwardVector = survivor->GetActorForwardVector();

    Speed = FVector::DotProduct(forwardVector, velocity);
}

void USAnimInstance::AnimNotify_SDamageEnd()
{
    State = ESurvivorState::IDLE;
    survivor->State = ESurvivorState::IDLE;
}

void USAnimInstance::AnimNotify_SHitFallEnd()
{
    falling = false;
}
