#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CHunterAnimInstance.generated.h"

UCLASS()
class IDENTITYN_API UCHunterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    class ACharacter* OwnerCharacter = nullptr;

};
