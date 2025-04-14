#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class IDENTITYN_API UCWeaponData : public UObject
{
	GENERATED_BODY()

private:
    friend class UCWeaponAsset;

public:
    FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
    FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
    FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
    FORCEINLINE class UCChargeAction* GetChargeAction() { return ChargeAction; }
    FORCEINLINE class UCSkill* GetSkill() { return Skill; }
    FORCEINLINE class UCDoAction* GetSkillAction() { return SkillAction; }

private:
    UPROPERTY()
    class ACAttachment* Attachment;

    UPROPERTY()
    class UCEquipment* Equipment;

    UPROPERTY()
    class UCDoAction* DoAction;

    UPROPERTY()
    class UCChargeAction* ChargeAction;

    UPROPERTY()
    class UCSkill* Skill;

    UPROPERTY()
    class UCDoAction* SkillAction;

};
