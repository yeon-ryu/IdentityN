#include "Hunters/Widgets/CLobbyWidget.h"
#include "Components/Button.h"
#include "Hunters/Characters/CHunterController.h"

void UCLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Button_Survivor->OnClicked.AddDynamic(this, &UCLobbyWidget::SelectedSurvivor);
    Button_Hunter->OnClicked.AddDynamic(this, &UCLobbyWidget::SelectedHunter);

}

void UCLobbyWidget::SelectedSurvivor()
{
    if (auto pc = GetOwningPlayer<ACHunterController>())
        pc->ServerRequestSpawn(EPlayerRole::Survivor);

}

void UCLobbyWidget::SelectedHunter()
{
    if (auto pc = GetOwningPlayer<ACHunterController>())
        pc->ServerRequestSpawn(EPlayerRole::Hunter);

}
