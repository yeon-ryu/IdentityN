#include "Hunters/Characters/CHunterController.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Hunters/CHunterGameMode.h"
#include "Net/UnrealNetwork.h"

void ACHunterController::SetLobbyUIRef(UUserWidget* InWidget)
{
    LobbyUI = InWidget;

}

void ACHunterController::ServerRequestSpawn_Implementation(EPlayerRole InRole)
{
    SelectedRole = InRole;

    // GameMode 호출하여 직접 Pawn 생성
    if (ACHunterGameMode* GM = GetWorld()->GetAuthGameMode<ACHunterGameMode>())
        GM->SpawnPlayerCharacter(this); // 직접 스폰

    // 클라에 UI 제거 명령 전송
    ClientRemoveLobbyUI();

}

void ACHunterController::ClientRemoveLobbyUI_Implementation()
{
    if (LobbyUI)
    {
        LobbyUI->RemoveFromParent();

        LobbyUI->SetVisibility(ESlateVisibility::Hidden);

        bShowMouseCursor = false;

        this->SetInputMode(FInputModeGameOnly());
    }

}

void ACHunterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACHunterController, SelectedRole);

}
