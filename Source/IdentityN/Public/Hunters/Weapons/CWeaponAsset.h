#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hunters/Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class IDENTITYN_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<class ACAttachment> AttachmentClass;

    UPROPERTY(EditAnywhere)
    FEquipmentData EquipmentData;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UCEquipment> EquipmentClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UCDoAction> DoActionClass;

    UPROPERTY(EditAnywhere)
    TArray<FDoActionData> DoActionDatas;

    UPROPERTY(EditAnywhere)
    TArray<FHitData> HitDatas;

public:
    UCWeaponAsset();

    void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);

};
