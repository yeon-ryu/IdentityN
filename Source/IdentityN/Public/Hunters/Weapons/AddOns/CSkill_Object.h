#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkill_Object.generated.h"

UCLASS()
class IDENTITYN_API ACSkill_Object : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSkill_Object();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
