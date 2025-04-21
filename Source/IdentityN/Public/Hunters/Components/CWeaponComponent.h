#pragma once

#include "CoreMinimal.h"
#include "Hunters/Components/CBaseComponent.h"
#include "Hunters/Components/IBindEnhancedInput.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Harpoon, MAX
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom) )
class IDENTITYN_API UCWeaponComponent
    : public UCBaseComponent
    , public IIBindEnhancedInput
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Action;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Skill;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Capture;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Telport;

private:
    UPROPERTY(EditAnywhere, Category = "DataAsset")
    class UCWeaponAsset* DataAssets[(int32)EWeaponType::MAX];

public:
    FORCEINLINE EWeaponType GetWeaponType() { return Type; }

public:
    FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::MAX; }
    FORCEINLINE bool IsHarpoonMode() { return Type == EWeaponType::Harpoon; }

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    virtual void OnBindEnhancedInputSystem(class UEnhancedInputComponent* InEnhancedInput) override;
		
private:
    bool IsDefaultMode();

public:
    class ACAttachment* GetAttachment();
    class UCEquipment* GetEquipment();
    class UCDoAction* GetDoAction();

    class UCSkill* GetSkill();

public:
    UFUNCTION(BlueprintCallable)
    class UCChargeAction* GetChargeAction();

public:
    void SetUnarmedMode();
    void SetHarpoonMode();

private:
    void DoAction(const struct FInputActionValue& InVal);

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_DoAction();
    void ServerRPC_DoAction_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_DoAction();
    void MulticastRPC_DoAction_Implementation();

public:
    UFUNCTION(BlueprintCallable)
    void ChargeAction_Pressed();

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_ChargeAction_Pressed();
    void ServerRPC_ChargeAction_Pressed_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_ChargeAction_Pressed();
    void MulticastRPC_ChargeAction_Pressed_Implementation();

public:
    UFUNCTION(BlueprintCallable)
    void ChargeAction_Released();

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_ChargeAction_Released();
    void ServerRPC_ChargeAction_Released_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_ChargeAction_Released();
    void MulticastRPC_ChargeAction_Released_Implementation();

private:
    void OnChargingSkill(const struct FInputActionValue& InVal);

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_OnChargingSkill();
    void ServerRPC_OnChargingSkill_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_OnChargingSkill();
    void MulticastRPC_OnChargingSkill_Implementation();

private:
    void OnShootSkill(const struct FInputActionValue& InVal);

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_OnShootSkill();
    void ServerRPC_OnShootSkill_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_OnShootSkill();
    void MulticastRPC_OnShootSkill_Implementation();

private:
    void OnTeleport(const struct FInputActionValue& InVal);

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_OnTeleport();
    void ServerRPC_OnTeleport_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_OnTeleport();
    void MulticastRPC_OnTeleport_Implementation();

public:    
    void OnCapture(const struct FInputActionValue& InVal);

protected:
    UFUNCTION(Server, Reliable)
    void ServerRPC_OnCapture();
    void ServerRPC_OnCapture_Implementation();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_OnCapture();
    void MulticastRPC_OnCapture_Implementation();

private:
    void OnChargeAction(const struct FInputActionValue& InVal);
    void OffChargeAction(const struct FInputActionValue& InVal);

private:
    void OnSelectAction(const struct FInputActionValue& InVal);
    void OnInitAction(const struct FInputActionValue& InVal);

private:
    void SetMode(EWeaponType InType);
    void ChangeType(EWeaponType InType);

public:
    FWeaponTypeChanged OnWeaponTypeChange;

private:
    EWeaponType Type = EWeaponType::MAX;

public:
    UPROPERTY(Replicated)
    bool bCanSkill = true;

    UPROPERTY(Replicated)
    bool bUsedSkill = false;

private:
    UPROPERTY()
    class UCWeaponData* Datas[(int32)EWeaponType::MAX];

    FTimerHandle handle;

    float ChargeTime = 0;

    bool bSelect = false;

public:
    class ACharacter* Candidate;

};
