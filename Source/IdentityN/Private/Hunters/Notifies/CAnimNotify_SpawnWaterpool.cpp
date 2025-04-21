#include "Hunters/Notifies/CAnimNotify_SpawnWaterpool.h"
#include "Global.h"
#include "Hunters/Characters/CHunter_Naiad.h"
#include "Hunters/Weapons/AddOns/CWaterpool.h"

FString UCAnimNotify_SpawnWaterpool::GetNotifyName_Implementation() const
{
    return "Waterpool";

}

void UCAnimNotify_SpawnWaterpool::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    if (ACHunter_Naiad* naiad = Cast<ACHunter_Naiad>(MeshComp->GetOwner()))
        naiad->SpawnWaterpool();

}
