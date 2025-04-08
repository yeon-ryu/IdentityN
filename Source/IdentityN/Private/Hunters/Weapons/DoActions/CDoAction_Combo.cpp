﻿#include "Hunters/Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Hunters/Components/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
    CheckTrue(DoActionDatas.Num() < 1);

    if (bEnable)
    {
        bEnable = false;
        bExist = true;

        return;
    }

    CheckFalse(State->IsDefaultMode());

    Super::DoAction();

    DoActionDatas[Index].DoAction(OwnerCharacter);

}

void UCDoAction_Combo::Begin_DoAction()
{
    Super::Begin_DoAction();
    CheckFalse(bExist);

    bExist = false;
    DoActionDatas[++Index].DoAction(OwnerCharacter);

}

void UCDoAction_Combo::End_DoAction()
{
    Super::End_DoAction();

    Index = 0;

}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
    Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
    CheckNull(InOther);

    for (ACharacter* hitted : Hitted)
        CheckTrue(hitted == InOther);

    Hitted.AddUnique(InOther);

    CheckTrue(HitDatas.Num() - 1 < Index);
    //HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
    Super::OnAttachmentEndCollision();

    float angle = -2.0f;
    ACharacter* candidate = nullptr;

    for (ACharacter* hitted : Hitted)
    {
        FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
        direction = direction.GetSafeNormal2D();


        FVector forward = FQuat(OwnerCharacter->GetActorRotation()).GetForwardVector();

        float dot = FVector::DotProduct(direction, forward);
        if (dot >= angle)
        {
            angle = dot;
            candidate = hitted;
        }
    }

    if (!!candidate)
    {
        FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
        FRotator target = FRotator(0, rotator.Yaw, 0);

        AController* controller = OwnerCharacter->GetController<AController>();
        controller->SetControlRotation(target);
    }

    Hitted.Empty();

}
