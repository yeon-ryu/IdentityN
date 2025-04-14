#include "Hunters/Components/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Weapons/CWeaponAsset.h"
#include "Hunters/Weapons/CWeaponData.h"
#include "Hunters/Weapons/CAttachment.h"
#include "Hunters/Weapons/CEquipment.h"
#include "Hunters/Weapons/CDoAction.h"
#include "Hunters/Weapons/CChargeAction.h"
#include "Hunters/Weapons/Skills/CSkill.h"
#include "Hunters/Weapons/AddOns/CSkill_Object.h"
#include "Hunters/Weapons/AddOns/CObject_SpearFishing.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    CHelpers::GetAsset<UCWeaponAsset>(&DataAssets[0], TEXT("/Script/IdentityN.CWeaponAsset'/Game/PJS/Weapons/DA_Harpoon.DA_Harpoon'"));

}

void UCWeaponComponent::BeginPlay()
{	
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    for (int32 i = 0; i < (int32)EWeaponType::MAX; i++)
    {
        if (!!DataAssets[i])
            DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
    }

    Super::BeginPlay();

    SetHarpoonMode();

}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!!GetDoAction())
        GetDoAction()->Tick(DeltaTime);

    if (!!GetChargeAction())
        GetChargeAction()->Tick(DeltaTime);

}

void UCWeaponComponent::OnBindEnhancedInputSystem(UEnhancedInputComponent* InEnhancedInput)
{
    //InEnhancedInput->BindAction(IA_Action, ETriggerEvent::Started, this, &UCWeaponComponent::DoAction);

    //InEnhancedInput->BindAction(IA_ChargeAction, ETriggerEvent::Triggered, this, &UCWeaponComponent::OnChargeAction);
    //InEnhancedInput->BindAction(IA_ChargeAction, ETriggerEvent::Completed, this, &UCWeaponComponent::OffChargeAction);

    InEnhancedInput->BindAction(IA_Action, ETriggerEvent::Triggered, this, &UCWeaponComponent::SelectAction);
    InEnhancedInput->BindAction(IA_Action, ETriggerEvent::Completed, this, &UCWeaponComponent::InitAction);

    InEnhancedInput->BindAction(IA_Skill, ETriggerEvent::Started, this, &UCWeaponComponent::ChargingSkill);
    InEnhancedInput->BindAction(IA_Skill, ETriggerEvent::Completed, this, &UCWeaponComponent::ShootSkill);

}

bool UCWeaponComponent::IsDefaultMode()
{
    return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsDefaultMode();

}

ACAttachment* UCWeaponComponent::GetAttachment()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetAttachment();

}

UCEquipment* UCWeaponComponent::GetEquipment()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetEquipment();

}

UCDoAction* UCWeaponComponent::GetDoAction()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetDoAction();

}

UCSkill* UCWeaponComponent::GetSkill()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetSkill();
}

UCChargeAction* UCWeaponComponent::GetChargeAction()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetChargeAction();

}

void UCWeaponComponent::SetUnarmedMode()
{
    GetEquipment()->Unequip();

    ChangeType(EWeaponType::MAX);

}

void UCWeaponComponent::SetHarpoonMode()
{
    CheckFalse(IsDefaultMode());

    SetMode(EWeaponType::Harpoon);

}

void UCWeaponComponent::DoAction(const struct FInputActionValue& InVal)
{
    if (!!GetDoAction())
        GetDoAction()->DoAction();

}

void UCWeaponComponent::ChargeAction_Pressed()
{
    if (!!GetChargeAction())
        GetChargeAction()->Pressed();

}

void UCWeaponComponent::ChargeAction_Released()
{
    if (!!GetChargeAction())
        GetChargeAction()->Released();

}

void UCWeaponComponent::OnChargeAction(const FInputActionValue& InVal)
{
    ChargeAction_Pressed();

}

void UCWeaponComponent::OffChargeAction(const FInputActionValue& InVal)
{
    ChargeAction_Released();

}

void UCWeaponComponent::SelectAction(const FInputActionValue& InVal)
{
    ChargeTime += GetWorld()->GetDeltaSeconds();

}

void UCWeaponComponent::InitAction(const FInputActionValue& InVal)
{
    if (ChargeTime >= 1)
    {
        ChargeTime = 0;

        OnChargeAction(FInputActionValue());
        OffChargeAction(FInputActionValue());
    }
    else DoAction(FInputActionValue());

}

void UCWeaponComponent::ChargingSkill(const FInputActionValue& InVal)
{
    if (bCanSkill)
    {
        if (!!GetSkill())
            GetSkill()->Pressed();

        bCanSkill = false;

        bUsedSkill = true;
    }
    else
    {
        if (ACObject_SpearFishing* object = CHelpers::FindActor<ACObject_SpearFishing>(GetWorld()))
            object->Return(GetOwner()->GetActorLocation());
    }

}

void UCWeaponComponent::ShootSkill(const FInputActionValue& InVal)
{
    if (!bCanSkill)
    {
        if (!!GetSkill())
            GetSkill()->Released();

        GetAttachment()->SetActorHiddenInGame(bUsedSkill);
    }

}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
    if (Type == InType)
    {
        SetUnarmedMode();

        return;
    }
    else if (IsUnarmedMode() == false)
    {
        GetEquipment()->Unequip();
    }

    if (!!Datas[(int32)InType])
    {
        Datas[(int32)InType]->GetEquipment()->Equip();

        ChangeType(InType);
    }

}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
    EWeaponType prevType = Type;
    Type = InType;

    if (OnWeaponTypeChange.IsBound())
        OnWeaponTypeChange.Broadcast(prevType, InType);

}
