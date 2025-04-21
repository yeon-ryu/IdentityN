// Fill out your copyright notice in the Description page of Project Settings.


#include "IdentityNPlayerController.h"
#include "IdentityNGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "Survivor/Characters/Embalmer.h"

void AIdentityNPlayerController::BeginPlay()
{
    Super::BeginPlay();
    gm = Cast<AIdentityNGameMode>(GetWorld()->GetAuthGameMode());

    // 이하 테스트 성공
    // 매개변수 감시자 enum 추가하고 (3번째 인자값) UI 에서 감시자인가 생존자인가 정하고 (첫번째 인자값)
    // 각 정한 UI 마다 Enum 띄워줘서 선택해서 아래 함수 부른다.
    // 아니면 그냥 감시자 생존자 부르는 함수 별개로 나누고 Enum 하나씩만 인자로 받아도 됨
    //ServerRPC_ChangePlayerPawn_Implementation(true, ESurvivorPawn::Embalmer);
}

void AIdentityNPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
    if (!IsLocalPlayerController()) return;

    APawn* player = GetPawn();

    if (player) {
        // 관전자 생성
        FActorSpawnParameters params;
        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        auto spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetActorTransform(), params);
        
        // 조종 변경
        UnPossess();
        Possess(spectator);

        player->Destroy();
    }
}

void AIdentityNPlayerController::ServerRPC_ChangePlayerPawn_Implementation(bool isSurvivor, ESurvivorPawn survivor)
{
    if (!IsLocalPlayerController()) return;

    APawn* player = GetPawn();
    if(player == nullptr) return;

    FActorSpawnParameters params;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (isSurvivor) {
        // 생존자 pawn 부르기
        switch (survivor) {
            case ESurvivorPawn::Embalmer :
                auto newPawn = GetWorld()->SpawnActor<AEmbalmer>(player->GetActorLocation(), player->GetActorRotation(), params);
                
                UnPossess();
                Possess(newPawn);
                player->Destroy();

                break;
        }
    }
    else {
        // 감시자 pawn 부르기

    }
}
