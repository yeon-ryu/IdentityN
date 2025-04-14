// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SBuff.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDENTITYN_API USBuff : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USBuff();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class ASurvivor* me;

public:
    // 버프에 사용되는 값들

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SBuff)
    float DamageSpeedPer = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SBuff)
    float DamageSpeedSeconds = 2.0f;

    // 3시 인격 - 마지막 희망
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SBuff)
    bool BorrowedTime = false;
	
public:
    // 버프 함수는 트리거 기준으로 나누기

    // 트리거 : 피격
    void DamagedBuff();

    // 트리거 : 모든 암호기 해독
    void CompleteDecode();

    void SetInitData();
};
