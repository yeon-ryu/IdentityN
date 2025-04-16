// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/SurvivorController.h"
#include "GameFramework/SpectatorPawn.h"
#include "IdentityNGameMode.h"

void ASurvivorController::BeginPlay()
{
    Super::BeginPlay();
    gm = Cast<AIdentityNGameMode>(GetWorld()->GetAuthGameMode());
}

void ASurvivorController::ServerRPC_ChangeToSpectator_Implementation()
{
    if(!IsLocalPlayerController()) return;

    APawn* player = GetPawn();

    if (player) {
        // 관전자 생성
        FActorSpawnParameters params;
        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        auto spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetActorTransform(), params);
        
        // 조종 변경
        Possess(spectator);

        player->Destroy();
    }
}
