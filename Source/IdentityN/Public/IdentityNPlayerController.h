// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Survivor/Data/SurvivorEnum.h"
#include "IdentityNPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IDENTITYN_API AIdentityNPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerRPC_ChangeToSpectator();

    UFUNCTION(Server, Reliable)
    void ServerRPC_ChangePlayerPawn(bool isSurvivor, ESurvivorPawn survivor);

private:
    UPROPERTY()
    class AIdentityNGameMode* gm;
};
