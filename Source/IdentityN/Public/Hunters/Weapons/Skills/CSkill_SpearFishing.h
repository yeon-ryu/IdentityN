#pragma once

#include "CoreMinimal.h"
#include "Hunters/Weapons/Skills/CSkill.h"
#include "Hunters/Weapons/CWeaponStructures.h"
#include "CSkill_SpearFishing.generated.h"

UCLASS(Blueprintable)
class IDENTITYN_API UCSkill_SpearFishing : public UCSkill
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AActor> ObjectClass;

    UPROPERTY(EditDefaultsOnly)
    FVector ObjectLocation;

private:
    UPROPERTY(EditDefaultsOnly)
    FDoActionData DoActionData;

public:
    UCSkill_SpearFishing();

public:
    virtual void Pressed() override;

    virtual void Begin_Skill_Implementation() override;
    virtual void End_Skill_Implementation() override;

    virtual void Tick_Implementation(float InDeltaTime) {}

};
