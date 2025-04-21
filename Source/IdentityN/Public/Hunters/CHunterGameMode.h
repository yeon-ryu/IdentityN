#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CHunterGameMode.generated.h"

UCLASS()
class IDENTITYN_API ACHunterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Class")
    TSubclassOf<APawn> SurvivorClass;

    UPROPERTY(EditDefaultsOnly, Category = "Class")
    TSubclassOf<APawn> HunterClass;

public:
    ACHunterGameMode();

public:
    UFUNCTION()
    void SpawnPlayerCharacter(AController* Controller);

};
