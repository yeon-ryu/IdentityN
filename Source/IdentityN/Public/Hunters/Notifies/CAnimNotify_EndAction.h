﻿#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndAction.generated.h"

UCLASS()
class IDENTITYN_API UCAnimNotify_EndAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    FString GetNotifyName_Implementation() const override;

    void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
