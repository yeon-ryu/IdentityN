#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRocketChair.generated.h"

UCLASS()
class IDENTITYN_API ACRocketChair : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* Box;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;
	
public:	
	ACRocketChair();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    UPROPERTY()
    class ASurvivor* survivor;

};
