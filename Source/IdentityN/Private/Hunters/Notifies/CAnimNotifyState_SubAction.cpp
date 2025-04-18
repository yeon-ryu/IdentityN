﻿#include "Hunters/Notifies/CAnimNotifyState_SubAction.h"
#include "Global.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Hunters/Weapons/CChargeAction.h"

FString UCAnimNotifyState_SubAction::GetNotifyName_Implementation() const
{
    return "ChargeAction";
}

void UCAnimNotifyState_SubAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetChargeAction());

    weapon->GetChargeAction()->Begin_ChargeAction();
}

void UCAnimNotifyState_SubAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetChargeAction());

    weapon->GetChargeAction()->End_ChargeAction();
}
