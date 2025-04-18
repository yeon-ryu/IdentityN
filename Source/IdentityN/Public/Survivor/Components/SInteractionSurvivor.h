// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionSurvivor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDENTITYN_API USInteractionSurvivor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionSurvivor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void SetupInputBinding(class UEnhancedInputComponent* input);

public:


    // 치료 게이지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gauge)
    float HealGauge = 0.0f;


private:
	UPROPERTY()
	class ASurvivor* me;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Heal;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Space;

    // 상처 여부 (HP 가 1 이하일 때 true 가 된다)
    bool bHurt = false;

    // 치료 받고 있는 중인지 (힐 받는 쪽 요소)
    bool bHealing = false;

    bool bHealMyself = false;

    // 치료할 생존자가 근처에 있는지 (힐 주는 쪽 요소)
    class ASurvivor* NearSurvivor;

    // 치료해주는 생존자 (힐 받는 쪽 요소)
    class ASurvivor* HealSurvivor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float HealTime = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float HealSelfTime = 30.0f;

public:
    void SetInitData();

    void StartHeal(class ASurvivor* survivor);

    void Heal();

    void HealMyself();

    void EndHeal();

    void RemoveHealer();

    void HealAction(const struct FInputActionValue& value);
    void HealMyselfAction(const struct FInputActionValue& value);

    UFUNCTION()
	void OnHealSearchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	void OnHealSearchEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
