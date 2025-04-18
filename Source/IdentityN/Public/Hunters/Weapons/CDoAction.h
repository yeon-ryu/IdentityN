﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Hunters/Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS()
class IDENTITYN_API UCDoAction : public UObject
{
	GENERATED_BODY()
	
public:
    FORCEINLINE bool GetBeginAction() { return bBeginAction; }
    FORCEINLINE bool GetInAction() { return bInAction; }

public:
    UCDoAction();

    virtual void BeginPlay
    (
        class ACAttachment* InAttachment,
        class UCEquipment* InEquipment,
        class ACharacter* InOwner,
        const TArray<FDoActionData>& InDoActionDatas,
        const TArray<FHitData>& InHitDatas
    );

    virtual void Tick(float InDeltaTime) {}

public:
    virtual void DoAction();
    virtual void Begin_DoAction();
    virtual void End_DoAction();

public:
    UFUNCTION()
    virtual void OnBeginEquip() {}

    UFUNCTION()
    virtual void OnUnequip() {}

public:
    UFUNCTION()
    virtual void OnAttachmentBeginCollision() {}

    UFUNCTION()
    virtual void OnAttachmentEndCollision() {}

    UFUNCTION()
    virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther) {}

    UFUNCTION()
    virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {}

protected:
    bool bInAction;
    bool bBeginAction;

    class ACharacter* OwnerCharacter;
    class UWorld* World;

    class UCMovementComponent* Movement;
    class UCStateComponent* State;

    TArray<FDoActionData> DoActionDatas;
    TArray<FHitData> HitDatas;

};
