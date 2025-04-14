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
    class UInputAction* IA_ChargeAction;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputAction* IA_Skill;

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

public:
    UFUNCTION(BlueprintCallable)
    void ChargeAction_Pressed();

    UFUNCTION(BlueprintCallable)
    void ChargeAction_Released();

private:
    void OnChargeAction(const struct FInputActionValue& InVal);
    void OffChargeAction(const struct FInputActionValue& InVal);

private:
    void SelectAction(const struct FInputActionValue& InVal);
    void InitAction(const struct FInputActionValue& InVal);

    void ChargingSkill(const struct FInputActionValue& InVal);
    void ShootSkill(const struct FInputActionValue& InVal);

private:
    void SetMode(EWeaponType InType);
    void ChangeType(EWeaponType InType);

public:
    FWeaponTypeChanged OnWeaponTypeChange;

private:
    EWeaponType Type = EWeaponType::MAX;

private:
    UPROPERTY()
    class UCWeaponData* Datas[(int32)EWeaponType::MAX];

    FTimerHandle handle;

    float ChargeTime = 0;

    bool bSelect = false;

public:
    bool bCanSkill = true;
    bool bUsedSkill = false;

};
