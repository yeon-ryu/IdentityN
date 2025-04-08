#include "Hunters/Weapons/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"
#include "Hunters/Weapons/CAttachment.h"
#include "Hunters/Weapons/CEquipment.h"

UCDoAction::UCDoAction()
{

}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
    OwnerCharacter = InOwner;
    World = OwnerCharacter->GetWorld();

    State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
    Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

    DoActionDatas = InDoActionDatas;
    HitDatas = InHitDatas;

}

void UCDoAction::DoAction()
{
    bInAction = true;

    State->SetActionMode();

}

void UCDoAction::Begin_DoAction()
{
    bBeginAction = true;

}

void UCDoAction::End_DoAction()
{
    bInAction = false;
    bBeginAction = false;

    State->SetDefaultMode();

    Movement->OnMovable();
    Movement->DisableFixedCamera();

}
