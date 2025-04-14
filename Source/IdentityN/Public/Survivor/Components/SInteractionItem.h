// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionItem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDENTITYN_API USInteractionItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void SetupInputBinding(class UEnhancedInputComponent* input);

private:
	UPROPERTY()
	class ASurvivor* me;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float DecodeTime = 81.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float DecodeFailPer = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float DecodeFailTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cipher, meta = (AllowPrivateAccess = "true"))
    class ACipherMachine* NearChipher;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cipher, meta = (AllowPrivateAccess = "true"))
    bool bNearChipher = false;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
    float OpenDoorTime = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door, meta = (AllowPrivateAccess = "true"))
    class ADoor* NearDoor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door, meta = (AllowPrivateAccess = "true"))
    bool bNearDoor = false;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Decode;

public:
    void Decode(const struct FInputActionValue& value);

    void EndDecode();
    void InCipherArea(class ACipherMachine* machine);
    void OutCipherArea();

    bool GetIsNearMachine();
    float GetDecodeTime();
    void SetDecodeTime(float time);

    bool GetIsNearDoor();
    float GetOpenTime();
    void EndOpen();
    void InDoorArea(class ADoor* door);
    void OutDoorArea();

    void SetInitData();
};
