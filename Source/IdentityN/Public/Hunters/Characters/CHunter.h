#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHunter.generated.h"

UCLASS(Abstract, NotBlueprintable)
class IDENTITYN_API ACHunter : public ACharacter
{
	GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere, Category = "Character")
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, Category = "Character")
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, Category = "Character")
    class USphereComponent* Fear;

    UPROPERTY(VisibleAnywhere, Category = "Character")
    class USplineComponent* Spline;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCStateComponent* State;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCMovementComponent* Movement;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCWeaponComponent* Weapon;

private:
    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    class UInputMappingContext* MappingContext;

public:
	ACHunter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void InitializeCharacters();

    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    UPROPERTY()
    class ACharacter* Target;

};
