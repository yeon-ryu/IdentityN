#include "Hunters/Components/CBaseComponent.h"
#include "Global.h"
#include "Hunters/Characters/CHunter.h"

UCBaseComponent::UCBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCBaseComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACHunter>(GetOwner());

}

void UCBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
