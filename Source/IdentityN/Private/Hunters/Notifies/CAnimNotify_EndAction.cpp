﻿#include "Hunters/Notifies/CAnimNotify_EndAction.h"
#include "Global.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Hunters/Weapons/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
    return "End_DoAction";

}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetDoAction());

    weapon->GetDoAction()->End_DoAction();

}
