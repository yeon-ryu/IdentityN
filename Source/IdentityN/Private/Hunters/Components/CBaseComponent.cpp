#include "Hunters/Components/CBaseComponent.h"
#include "Global.h"

UCBaseComponent::UCBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCBaseComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
