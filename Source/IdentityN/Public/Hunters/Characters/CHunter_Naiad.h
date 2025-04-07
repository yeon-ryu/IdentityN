#pragma once

#include "CoreMinimal.h"
#include "Hunters/Characters/CHunter.h"
#include "CHunter_Naiad.generated.h"

UCLASS(Blueprintable)
class IDENTITYN_API ACHunter_Naiad : public ACHunter
{
	GENERATED_BODY()
	
public:
    ACHunter_Naiad();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
