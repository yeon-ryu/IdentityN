// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivorController.generated.h"

/**
 * 
 */
UCLASS()
class IDENTITYN_API ASurvivorController : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerRPC_ChangeToSpectator();

private:
    UPROPERTY()
    class AIdentityNGameMode* gm;
};
