// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/SurvivorEnum.h"
#include "../Data/SurvivorStruct.h"
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	class USAnimInstance* AnimInstance;

	UPROPERTY( EditAnywhere )
	class USMove* MoveComp;

    UPROPERTY( EditAnywhere )
	class USBuff* BuffComp;
    
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

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    ESurvivorState State = ESurvivorState::IDLE;

    FSurvivorData* SurvivorData;

    // 의자 게이지 : 0 보다 크고 50미만일 때 의자에 다시 앉으면 시작을 50으로 세팅, 50이상일 때 다시 앉으면 바로 100 으로
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    float SitGauge = 0.0f;

    // 쓰러짐 상태일때 사망까지 게이지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    float DeadGauge = 0.0f;

    // 치료 게이지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    float HealGauge = 0.0f;

    // 쓰러짐 상태 여부
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    bool bCrawl = false;

    // 무적 상태 여부
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    bool bInvindibility = false;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    int32 Id = -1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    FString Name = "";

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Survivor)
    float MaxHP = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    float HP = MaxHP;

    // 과다 출혈 사망 시간
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Survivor)
    float CrawlDeadTime = 60.0f;

private:
    // 과다 출혈로 죽을 때까지 남은 시간
    float CrawlCurrentTime = 0.0f;

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    
    FORCEINLINE int32 GetSurvivorId() const { return Id; }

    FORCEINLINE FString GetSurvivorName() const { return Name; }

    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
    /** Called for looking input */
    void Look(const struct FInputActionValue& Value);

    bool IsTakeDamage();

    bool IsTakeAction();

private:
    void SetInitData();

    void ProcessDeadGuage();
};
