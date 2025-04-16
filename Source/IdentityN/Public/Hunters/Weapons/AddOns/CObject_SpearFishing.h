#pragma once

#include "CoreMinimal.h"
#include "Hunters/Weapons/AddOns/CSkill_Object.h"
#include "CObject_SpearFishing.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACObject_SpearFishing*, InDestroyer);

UCLASS()
class IDENTITYN_API ACObject_SpearFishing : public ACSkill_Object
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly)
    class USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly)
    class USkeletalMeshComponent* SkeletalMesh;

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    class UCapsuleComponent* Capsule;

private:
    UPROPERTY(VisibleDefaultsOnly)
    class UProjectileMovementComponent* Projectile;

public:
    FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:
    ACObject_SpearFishing();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

public:
    void Return(FVector InDestination);

private:
    UFUNCTION()
    void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    FProjectileHit OnHit;
    FProjectileEndPlay OnEndPlay;

private:
    TArray<AActor*> Ignores;

    bool bReturn = false;

};
