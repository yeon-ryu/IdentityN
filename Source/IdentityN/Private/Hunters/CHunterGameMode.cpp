#include "Hunters/CHunterGameMode.h"
#include "Global.h"
#include "GameFramework/PlayerStart.h"
#include "Hunters/Characters/CHunterController.h"
#include "EngineUtils.h"

ACHunterGameMode::ACHunterGameMode()
{
    bStartPlayersAsSpectators = true;

    ConstructorHelpers::FClassFinder<APlayerController> controller(TEXT("/Script/Engine.Blueprint'/Game/PJS/Characters/BP_CHunterController.BP_CHunterController_C'"));
    if (controller.Succeeded())
        PlayerControllerClass = controller.Class;

}

void ACHunterGameMode::SpawnPlayerCharacter(AController* Controller)
{
    if (ACHunterController* PC = Cast<ACHunterController>(Controller))
    {
        FName DesiredTag = (PC->SelectedRole == EPlayerRole::Hunter) ? "HunterStart" : "SurvivorStart";

        APlayerStart* Start = nullptr;
        for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        {
            if (It->PlayerStartTag == DesiredTag)
            {
                Start = *It;

                break;
            }
        }

        if (Start)
        {
            TSubclassOf<APawn> ClassToSpawn = (PC->SelectedRole == EPlayerRole::Hunter) ? HunterClass : SurvivorClass;

            if (ClassToSpawn)
            {
                APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ClassToSpawn, Start->GetActorLocation(), Start->GetActorRotation());
                if (IsValid(SpawnedPawn))
                    Controller->Possess(SpawnedPawn);
            }

            Start->Destroy();
        }
    }

}
