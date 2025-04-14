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

    static ConstructorHelpers::FObjectFinder<UDataTable>TempSPersona(TEXT("/Script/Engine.DataTable'/Game/RGY/Data/SurvivorPersona.SurvivorPersona'"));

    if (TempSPersona.Succeeded()) {
        SurvivorPersonaTable = TempSPersona.Object;
    }
    else {
        CLog::Log("Fail Load SurvivorPersonaTable.");
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

void UIdentityNGameInstance::ReadSurvivorPersona()
{
    if (SurvivorPersonaTable) {
        TArray<FSurvivorPersona*> dataList;
        SurvivorPersonaTable->GetAllRows(FString(), dataList);

        for (FSurvivorPersona* sd : dataList) {
            SurvivorPersonaMap.Add(sd->Id, *sd);
        }
    }
    else {
        CLog::Log("Fail Binding SurvivorPersonaMap.");
    }
}
