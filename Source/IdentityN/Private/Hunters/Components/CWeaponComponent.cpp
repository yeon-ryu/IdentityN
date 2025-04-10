﻿#include "Hunters/Components/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Weapons/CWeaponAsset.h"
#include "Hunters/Weapons/CWeaponData.h"
#include "Hunters/Weapons/CAttachment.h"
#include "Hunters/Weapons/CEquipment.h"
#include "Hunters/Weapons/CDoAction.h"
#include "Hunters/Weapons/CSubAction.h"
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

    if (!!GetSubAction())
        GetSubAction()->Tick(DeltaTime);

}

void UCWeaponComponent::OnBindEnhancedInputSystem(UEnhancedInputComponent* InEnhancedInput)
{
    //InEnhancedInput->BindAction(IA_Action, ETriggerEvent::Started, this, &UCWeaponComponent::DoAction);

    //InEnhancedInput->BindAction(IA_SubAction, ETriggerEvent::Triggered, this, &UCWeaponComponent::OnSubAction);
    //InEnhancedInput->BindAction(IA_SubAction, ETriggerEvent::Completed, this, &UCWeaponComponent::OffSubAction);

    InEnhancedInput->BindAction(IA_Action, ETriggerEvent::Triggered, this, &UCWeaponComponent::SelectAction);
    InEnhancedInput->BindAction(IA_Action, ETriggerEvent::Completed, this, &UCWeaponComponent::InitAction);

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

UCSubAction* UCWeaponComponent::GetSubAction()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetSubAction();

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

void UCWeaponComponent::SubAction_Pressed()
{
    if (!!GetSubAction())
        GetSubAction()->Pressed();

}

void UCWeaponComponent::SubAction_Released()
{
    if (!!GetSubAction())
        GetSubAction()->Released();

}

void UCWeaponComponent::OnSubAction(const FInputActionValue& InVal)
{
    SubAction_Pressed();

}

void UCWeaponComponent::OffSubAction(const FInputActionValue& InVal)
{
    SubAction_Released();

}

void UCWeaponComponent::SelectAction(const FInputActionValue& InVal)
{
    //if (!GetWorld()->GetTimerManager().IsTimerActive(handle))
    //    return;

    //auto lambda = [&]() { bSelect = true; };

    //GetWorld()->GetTimerManager().SetTimer(handle, lambda, 1, false);

    ChargetTime += GetWorld()->GetDeltaSeconds();

}

void UCWeaponComponent::InitAction(const FInputActionValue& InVal)
{
    //if (GetWorld()->GetTimerManager().IsTimerActive(handle))
    //    GetWorld()->GetTimerManager().ClearTimer(handle);

    //if (bSelect)
    //{
    //    bSelect = false;

    //    OnSubAction(FInputActionValue());
    //    OffSubAction(FInputActionValue());
    //}
    //else DoAction(FInputActionValue());

    if (ChargetTime >= 1)
    {
        ChargetTime = 0;

        OnSubAction(FInputActionValue());
        OffSubAction(FInputActionValue());
    }
    else DoAction(FInputActionValue());

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
