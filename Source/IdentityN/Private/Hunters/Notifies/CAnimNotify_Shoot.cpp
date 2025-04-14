#include "Hunters/Notifies/CAnimNotify_Shoot.h"
#include "Global.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Hunters/Weapons/AddOns/CObject_SpearFishing.h"

FString UCAnimNotify_Shoot::GetNotifyName_Implementation() const
{
    return "Shoot";

}

void UCAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
    CheckNull(weapon);
    CheckNull(weapon->GetDoAction());

}
