#include "Hunters/Weapons/CSubAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"

UCSubAction::UCSubAction()
{

}

void UCSubAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
    Owner = InOwner;
    Attachment = InAttachment;
    DoAction = InDoAction;

    State = CHelpers::GetComponent<UCStateComponent>(Owner);
    Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);

}

void UCSubAction::Pressed()
{
    bInAction = true;

}

void UCSubAction::Released()
{
    bInAction = false;

}
