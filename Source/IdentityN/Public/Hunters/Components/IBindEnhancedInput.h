#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBindEnhancedInput.generated.h"

UINTERFACE(MinimalAPI)
class UIBindEnhancedInput : public UInterface
{
	GENERATED_BODY()

};

class IDENTITYN_API IIBindEnhancedInput
{
	GENERATED_BODY()

public:
    virtual void OnBindEnhancedInputSystem(class UEnhancedInputComponent* InEnhancedInput) = 0;

};
