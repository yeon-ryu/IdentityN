#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CLobbyPawn.generated.h"

UCLASS()
class IDENTITYN_API ACLobbyPawn : public APawn
{
	GENERATED_BODY()

public:
	ACLobbyPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
