#include "Hunters/Weapons/CChargeAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"

UCChargeAction::UCChargeAction()
{

}

void UCChargeAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
    Owner = InOwner;
    Attachment = InAttachment;
    DoAction = InDoAction;

    State = CHelpers::GetComponent<UCStateComponent>(Owner);
    Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);

}

void UCChargeAction::Pressed()
{
    bInAction = true;

}

void UCChargeAction::Released()
{
    bInAction = false;

}
