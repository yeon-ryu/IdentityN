#include "Hunters/Components/CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCStateComponent::SetDefaultMode()
{
    ChangeType(EStateType::Default);

}

void UCStateComponent::SetEquipMode()
{
    ChangeType(EStateType::Equip);

}

void UCStateComponent::SetActionMode()
{
    ChangeType(EStateType::Action);

}

void UCStateComponent::SetDamagedMode()
{
    ChangeType(EStateType::Damaged);

}

void UCStateComponent::OnChargeActionMode()
{
    bInChargeActionMode = true;

}

void UCStateComponent::OffChargeActionMode()
{
    bInChargeActionMode = false;

}

void UCStateComponent::ChangeType(EStateType InType)
{
    EStateType prevType = Type;
    Type = InType;

    if (OnStateTypeChanged.IsBound())
        OnStateTypeChanged.Broadcast(prevType, Type);

}
