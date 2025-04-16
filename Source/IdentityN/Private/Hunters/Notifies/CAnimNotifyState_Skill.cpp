#include "Hunters/Notifies/CAnimNotifyState_Skill.h"
#include "Global.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Hunters/Weapons/Skills/CSkill.h"

FString UCAnimNotifyState_Skill::GetNotifyName_Implementation() const
{
    return "Skill";

}

void UCAnimNotifyState_Skill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetChargeAction());

    weapon->GetSkill()->Begin_Skill();

}

void UCAnimNotifyState_Skill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetChargeAction());

    weapon->GetSkill()->End_Skill();

}
