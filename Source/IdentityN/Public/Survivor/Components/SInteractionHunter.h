// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionHunter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDENTITYN_API USInteractionHunter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionHunter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gauge, meta = (AllowPrivateAccess = "true"))
    float BalloonGauge = 0.0f;

private:
	UPROPERTY()
	class ASurvivor* me;

    bool bNearHunter = false;

    class ACHunter* NearHunter;

    class ACHunter* CatchHunter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float BalloonedFallTime = 16.0f;

public:
    void CheckDistanceHunter();

    void CatchBallooned(class ACHunter* hunter);

    void ReleaseBallooned();

    void EscapeBallooned();

    void TryEscapeBallooned(float strength);

    void SetInitData();

private:
    UFUNCTION()
	void OnSenseOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	void OnSenseEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
