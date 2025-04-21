#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SpawnWaterpool.generated.h"

UCLASS()
class IDENTITYN_API UCAnimNotify_SpawnWaterpool : public UAnimNotify
{
	GENERATED_BODY()

public:
    FString GetNotifyName_Implementation() const override;

    void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
