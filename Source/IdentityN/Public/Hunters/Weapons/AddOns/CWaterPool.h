#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWaterpool.generated.h"

UCLASS()
class IDENTITYN_API ACWaterPool : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* Collision;

    UPROPERTY(VisibleAnywhere)
    class UNiagaraComponent* OpeningPoolEffect;

    UPROPERTY(VisibleAnywhere)
    class UNiagaraComponent* ClosingPoolEffect;
	
public:	
    ACWaterPool();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    void InitializePool(class ACHunter_Naiad* InOwner, int32 InIndex);

    void ActivateSwampEffect();

private:
    // 습지 전환
    void TryFormSwampFromThis();
    void CollectConnectedPoolsRecursive(ACWaterPool* From, TSet<ACWaterPool*>& OutSet);

    // 생명 연장
    void ExtendLifeSpan();

private:
    UPROPERTY()
    TSet<ACWaterPool*> ConnectedPools;

    UPROPERTY()
    class ACHunter_Naiad* OwnerCharacter;

private:
    const float InitialLifeSpan = 20;

    bool bLifeExtended = false;
    const float ExtendedLifeSpan = 30;

    int32 Index = -1;

    bool bInSwamp = false;

};
