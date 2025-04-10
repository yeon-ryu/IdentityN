// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SMove.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDENTITYN_API USMove : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class ASurvivor* me;

	UPROPERTY()
	class UCharacterMovementComponent* MoveComp;

public:
    bool bCrouch = false;
    bool bCrawl = false;

private:
    /** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Move;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Crouch;

    float runSpeed = 380.0f;
    float crouchSpeed = 114.0f;
    float crawlSpeed = 44.0f;

public:
	void SetupInputBinding(class UEnhancedInputComponent* input);

    /** Called for movement input */
    void Move(const struct FInputActionValue& Value);
    
    void CrouchToggle(const struct FInputActionValue& Value);

    void ChangeCrouch(bool crouchState);

    void ChangeCrawl(bool crawlState);

    void ResetSpeed();

    void BuffSpeed(float per, int seconds);

private:
    void SetInitData();
};
