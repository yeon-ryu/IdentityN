// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

UCLASS()
class IDENTITYN_API ASurvivor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void NotifyControllerChanged() override;

public:
	UPROPERTY( EditAnywhere )
	class USMove* MoveComp;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* IMC_Survivor;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Look;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    int32 PlayerId = -1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    float MaxHP = 2.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    float HP = MaxHP;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    bool bCrawl = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    bool CrawlTime = 0.0f;
    
    // 생존자 상태 Enum 으로 만들고 준 후 public 에서 Get 으로 조회 가능하도록

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    
    FORCEINLINE int32 GetPlayerId() const { return PlayerId; }

    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
    /** Called for looking input */
    void Look(const struct FInputActionValue& Value);
};
