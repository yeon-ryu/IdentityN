#include "Hunters/CHunterGameMode.h"
#include "Global.h"

ACHunterGameMode::ACHunterGameMode()
{
    ConstructorHelpers::FClassFinder<APawn> pawn(TEXT("/Script/Engine.Blueprint'/Game/PJS/Characters/BP_CHunter_Naiad.BP_CHunter_Naiad_C'"));
    if (pawn.Succeeded())
        DefaultPawnClass = pawn.Class;

}
