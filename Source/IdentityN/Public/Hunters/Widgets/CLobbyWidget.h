#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLobbyWidget.generated.h"

UCLASS()
class IDENTITYN_API UCLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button_Survivor;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button_Hunter;

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void SelectedSurvivor();

    UFUNCTION()
    void SelectedHunter();

};
