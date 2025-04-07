#include "Hunters/Characters/CHunterAnimInstance.h"
#include "Global.h"

void UCHunterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwnerCharacter = Cast<ACharacter>(GetOwningActor());
    CheckNull(OwnerCharacter);

}

void UCHunterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

}
