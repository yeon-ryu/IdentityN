#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CHunterController.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    Survivor, Hunter, MAX
};

UCLASS()
class IDENTITYN_API ACHunterController : public APlayerController
{
	GENERATED_BODY()

private:
    UPROPERTY()
    class UUserWidget* LobbyUI;

public:
    UFUNCTION(BlueprintCallable)
    void SetLobbyUIRef(UUserWidget* InWidget);

    UFUNCTION(Server, Reliable)
    void ServerRequestSpawn(EPlayerRole InRole);
    void ServerRequestSpawn_Implementation(EPlayerRole InRole);

    UFUNCTION(Client, Reliable)
    void ClientRemoveLobbyUI();
    void ClientRemoveLobbyUI_Implementation();

public:
    UPROPERTY(Replicated)
    EPlayerRole SelectedRole;

};
