﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCuaser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

UCLASS()
class IDENTITYN_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    class USceneComponent* Root;

public:
    ACAttachment();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintNativeEvent)
    void OnBeginEquip();
    virtual void OnBeginEquip_Implementation() {}

    UFUNCTION(BlueprintNativeEvent)
    void OnUnequip();
    virtual void OnUnequip_Implementation() {}

public:
    void OnCollisions();
    void OffCollisions();

private:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    UFUNCTION(BlueprintCallable, Category = "Attach")
    void AttachTo(FName InSocketName);

    UFUNCTION(BlueprintCallable, Category = "Attach")
    void AttachToCollision(FName InCollisionName);

public:
    FAttachmentBeginCollision OnAttachmentBeginCollision;
    FAttachmentEndCollision OnAttachmentEndCollision;

    FAttachmentBeginOverlap OnAttachmentBeginOverlap;
    FAttachmentEndOverlap OnAttachmentEndOverlap;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Game")
    class ACharacter* OwnerCharacter;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    TArray<class UShapeComponent*> Collisions;

};
