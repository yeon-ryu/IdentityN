#include "Hunters/Characters/CLobbyPawn.h"
#include "Global.h"

ACLobbyPawn::ACLobbyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACLobbyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACLobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACLobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
