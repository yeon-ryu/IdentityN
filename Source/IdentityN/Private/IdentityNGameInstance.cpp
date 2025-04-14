// Fill out your copyright notice in the Description page of Project Settings.


#include "IdentityNGameInstance.h"
#include "Engine/DataTable.h"
#include "Utilities/CLog.h"
#include "UObject/ConstructorHelpers.h"

UIdentityNGameInstance::UIdentityNGameInstance()
{
    static ConstructorHelpers::FObjectFinder<UDataTable>TempSData(TEXT("/Script/Engine.DataTable'/Game/RGY/Data/SurvivorData.SurvivorData'"));

    if (TempSData.Succeeded()) {
        SurvivorDataTable = TempSData.Object;
    }
    else {
        CLog::Log("Fail Load SurvivorDataTable.");
    }
}

void UIdentityNGameInstance::Init()
{
    Super::Init();

    ReadSurvivorData();
}

void UIdentityNGameInstance::ReadSurvivorData()
{
    if (SurvivorDataTable) {
        TArray<FSurvivorData*> dataList;
        SurvivorDataTable->GetAllRows(FString(), dataList);

        for (FSurvivorData* sd : dataList) {
            SurvivorDataMap.Add(sd->Id, *sd);
        }
    }
    else {
        CLog::Log("Fail Binding SurvivorDataMap.");
    }
}
