// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Animations/SAnimInstance.h"
#include "Survivor/Characters/Survivor.h"

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ASurvivor* survivor = Cast<ASurvivor>(TryGetPawnOwner());
    if (survivor == nullptr) {
        return;
    }

    FVector velocity = survivor->GetVelocity();
    FVector forwardVector = survivor->GetActorForwardVector();

    Speed = FVector::DotProduct(forwardVector, velocity);
}
