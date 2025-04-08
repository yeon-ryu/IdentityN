#pragma once

#include "CoreMinimal.h"
#include "Hunters/Components/CBaseComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
    Default = 0, Equip, Action, Damaged, MAX
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom) )
class IDENTITYN_API UCStateComponent : public UCBaseComponent
{
	GENERATED_BODY()

public:
    FORCEINLINE bool IsDefaultMode() { return Type == EStateType::Default; }
    FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
    FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
    FORCEINLINE bool IsHittedMode() { return Type == EStateType::Damaged; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
public:
    void SetDefaultMode();
    void SetEquipMode();
    void SetActionMode();
    void SetDamagedMode();

private:
    void ChangeType(EStateType InType);

public:
    FStateTypeChanged OnStateTypeChanged;

private:
    EStateType Type = EStateType::Default;

private:
    bool bInSubActionMode;

};
