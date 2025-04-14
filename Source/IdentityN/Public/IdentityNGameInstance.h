// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Survivor/Data/SurvivorStruct.h"
#include "IdentityNGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class IDENTITYN_API UIdentityNGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UIdentityNGameInstance();
    virtual void Init() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    TMap<int32, FSurvivorData> SurvivorDataMap;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataTable, meta = (AllowPrivateAccess = "true"))
    class UDataTable* SurvivorDataTable;

private:
    void ReadSurvivorData();
};
