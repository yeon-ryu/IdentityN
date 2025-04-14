#include "Hunters/Weapons/Skills/CSkill.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"

UCSkill::UCSkill()
{

}

void UCSkill::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCSkill* InSkill)
{
    Owner = InOwner;
    Attachment = InAttachment;
    Skill = InSkill;

    State = CHelpers::GetComponent<UCStateComponent>(Owner);
    Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);

}

void UCSkill::Pressed()
{
    bInAction = true;

}

void UCSkill::Released()
{
    bInAction = false;

}
