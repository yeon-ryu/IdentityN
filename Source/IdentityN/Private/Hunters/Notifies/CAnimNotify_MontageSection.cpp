#include "Hunters/Notifies/CAnimNotify_MontageSection.h"
#include "Global.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_MontageSection::GetNotifyName_Implementation() const
{
    return "MontageSection";

}

void UCAnimNotify_MontageSection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    if (ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner()))
        character->PlayAnimMontage(character->GetCurrentMontage(), 1, *Name);
    
}
