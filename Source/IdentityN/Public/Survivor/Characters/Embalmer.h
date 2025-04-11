// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survivor/Characters/Survivor.h"
#include "Embalmer.generated.h"

/**
 * 
 */
UCLASS()
class IDENTITYN_API AEmbalmer : public ASurvivor
{
	GENERATED_BODY()
	
public:
    AEmbalmer();

    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
    class UStaticMeshComponent* Bag;

protected:
    virtual void BeginPlay() override;
};
