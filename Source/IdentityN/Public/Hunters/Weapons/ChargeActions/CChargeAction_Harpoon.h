#pragma once

#include "CoreMinimal.h"
#include "Hunters/Weapons/CChargeAction.h"
#include "Hunters/Weapons/CWeaponStructures.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CChargeAction_Harpoon.generated.h"

UCLASS(Blueprintable)
class IDENTITYN_API UCChargeAction_Harpoon : public UCChargeAction
{
	GENERATED_BODY()
	
private:
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float Distance = 1000;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float Speed = 200;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
    FDoActionData ActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Action")
    FHitData HitData;

public:
    void Pressed() override;
    void Begin_ChargeAction_Implementation() override;
    void End_ChargeAction_Implementation() override;
    void Tick_Implementation(float InDeltaTime) override;

private:
    UFUNCTION()
    void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther);

private:
    bool bMoving;

    FVector Start;
    FVector End;

    TArray<class ACharacter*> Overlapped;
    TArray<class ACharacter*> Hitted;

};
