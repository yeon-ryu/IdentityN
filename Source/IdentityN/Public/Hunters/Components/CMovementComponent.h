﻿#pragma once

#include "CoreMinimal.h"
#include "Hunters/Components/CBaseComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
    BaseWalk = 0, Spearfishing, MAX
};

UCLASS( ClassGroup=(Custom) )
class IDENTITYN_API UCMovementComponent
    : public UCBaseComponent
    , public IIBindEnhancedInput
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Movement;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Look;

public:
    FORCEINLINE bool CanMove() { return bMovable; }

    FORCEINLINE float GetBaseWalkSpeed() { return Speed[(int32)ESpeedType::BaseWalk]; }
    FORCEINLINE float GetSpearFishingSpeed() { return Speed[(int32)ESpeedType::Spearfishing]; }

    FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
    FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
    FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

private:
    UPROPERTY()
    float Speed[(int32)ESpeedType::MAX] = { 464, 603.2 };

private:
    UPROPERTY(EditAnywhere, Category = "CameraSpeed")
    float HorizontalLook = 0.5;

    UPROPERTY(EditAnywhere, Category = "CameraSpeed")
    float VerticalLook = 0.5;

public:	
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    virtual void OnBindEnhancedInputSystem(class UEnhancedInputComponent* InEnhancedInput) override;

public:
    void SetSpeed(ESpeedType InSpeed);

    void OnBaseWalk();
    void OnSpearfishing();

    void EnableControlRotation();
    void DisableControlRotation();

    void OnMovable();
    void OffMovable();

private:
    void OnMovement(const struct FInputActionValue& InVal);
    void OnLook(const struct FInputActionValue& InVal);

private:
    bool bMovable = true;
    bool bFixedCamera;

};
