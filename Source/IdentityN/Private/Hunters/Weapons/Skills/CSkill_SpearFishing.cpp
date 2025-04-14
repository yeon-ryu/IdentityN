#include "Hunters/Weapons/Skills/CSkill_SpearFishing.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"

UCSkill_SpearFishing::UCSkill_SpearFishing()
{

}

void UCSkill_SpearFishing::Pressed()
{
    Super::Pressed();

    CheckFalse(State->IsDefaultMode());
    State->SetActionMode();

    DoActionData.DoAction(Owner);

}

void UCSkill_SpearFishing::Begin_Skill_Implementation()
{
    Super::Begin_Skill_Implementation();

    FActorSpawnParameters params;
    params.Owner = Owner;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform transform;

    FVector ownerLocation = Owner->GetActorLocation() - Owner->GetActorUpVector() * 150;
    ownerLocation += Owner->GetActorRotation().RotateVector(ObjectLocation);

    transform.SetLocation(ownerLocation);

    Owner->GetWorld()->SpawnActor<AActor>(ObjectClass, transform, params);

}

void UCSkill_SpearFishing::End_Skill_Implementation()
{
    Super::End_Skill_Implementation();

    State->SetDefaultMode();
    Movement->OnMovable();

}
