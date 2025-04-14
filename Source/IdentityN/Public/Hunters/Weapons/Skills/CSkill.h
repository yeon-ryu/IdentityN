#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSkill.generated.h"

UCLASS()
class IDENTITYN_API UCSkill : public UObject
{
	GENERATED_BODY()
	
public:
    UCSkill();

public:
    virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCSkill* InSkill);

public:
    virtual void Pressed();
    virtual void Released();

public:
    UFUNCTION(BlueprintNativeEvent)
    void Begin_Skill();
    virtual void Begin_Skill_Implementation() {}

    UFUNCTION(BlueprintNativeEvent)
    void End_Skill();
    virtual void End_Skill_Implementation() {}

    UFUNCTION(BlueprintNativeEvent)
    void Tick(float InDeltaTime);
    virtual void Tick_Implementation(float InDeltaTime) {}

protected:
    bool bInAction;

    class ACharacter* Owner;
    class ACAttachment* Attachment;
    class UCSkill* Skill;

    class UCStateComponent* State;
    class UCMovementComponent* Movement;

};
