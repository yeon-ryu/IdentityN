#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHunter.generated.h"

USTRUCT(BlueprintType)
struct FHunterCharacteristics : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere)
    FString SkeletalMesh;

    UPROPERTY(VisibleAnywhere)
    FString AnimInstanceClass;

    UPROPERTY(VisibleAnywhere)
    float BasicSpeed;

    UPROPERTY(VisibleAnywhere)
    float SkillSpeed = 1;

    UPROPERTY(VisibleAnywhere)
    float FearRadius;

};

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

    virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    void EscapeSurvivor();

protected:
    void LoadHunterDataFromTable();

private:
    void InitializeCharacters();

protected:
    FHunterCharacteristics* Characteristics;

private:
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    class UDataTable* CharacteristicsTable;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Characteristic")
    FName HunterName;

};
