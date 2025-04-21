#include "Hunters/Characters/CHunter.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Hunters/Characters/CHunterAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/DataTable.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Survivor/Characters/Survivor.h"

ACHunter::ACHunter()
{
	PrimaryActorTick.bCanEverTick = true;

    InitializeCharacters();

}

void ACHunter::BeginPlay()
{
	Super::BeginPlay();

    if (APlayerController* controller = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()))
            subsystem->AddMappingContext(MappingContext, 0);
    }

}

void ACHunter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    LoadHunterDataFromTable();

}

void ACHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Movement
        Movement->OnBindEnhancedInputSystem(EnhancedInput);

        // Movement
        Weapon->OnBindEnhancedInputSystem(EnhancedInput);
    }

}

void ACHunter::EscapeSurvivor()
{
    Weapon->OnCapture(FInputActionValue());
    
}

void ACHunter::LoadHunterDataFromTable()
{
    CheckNull(CharacteristicsTable);
    Characteristics = CharacteristicsTable->FindRow<FHunterCharacteristics>(HunterName, TEXT("Hunter Init"));

    CheckNull(Characteristics);

    // SkeletalMesh
    if (USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, *Characteristics->SkeletalMesh))
        GetMesh()->SetSkeletalMesh(mesh);

    // AnimInstance
    if (UClass* anim = Cast<UClass>(StaticLoadClass(UCHunterAnimInstance::StaticClass(), nullptr, *Characteristics->AnimInstanceClass)))
        GetMesh()->SetAnimClass(anim);

    CheckNull(Movement);
    Movement->Speed[0] = Characteristics->BasicSpeed;
    Movement->Speed[1] = Characteristics->SkillSpeed;

    Fear->SetSphereRadius(Characteristics->FearRadius);

}

void ACHunter::InitializeCharacters()
{
    // Capsule
    GetCapsuleComponent()->SetRelativeScale3D(FVector(1.3));

    // Mesh
    GetMesh()->SetRelativeLocation(FVector(0, 0, ~110 + 1));
    GetMesh()->SetRelativeRotation(FQuat(FRotator(0, 270, 0)));
    GetMesh()->SetRelativeScale3D(FVector(3));

    // SpringArm
    CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", RootComponent);
    SpringArm->SetRelativeLocation(FVector(0, 0, 90));
    SpringArm->TargetArmLength = 200;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bEnableCameraLag = true;

    // Camera
    CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
    Camera->SetRelativeLocation(FVector(~120 + 1, 0, 0));
    Camera->bUsePawnControlRotation = false;

    // Characteristic DataTable
    CHelpers::GetAsset<UDataTable>(&CharacteristicsTable, TEXT("/Script/Engine.DataTable'/Game/PJS/Characters/DT_HunterCharacteristicsTable.DT_HunterCharacteristicsTable'"));

    // Fear
    CHelpers::CreateComponent<USphereComponent>(this, &Fear, "Fear", RootComponent);

    // Mapping Context
    CHelpers::GetAsset<UInputMappingContext>(&MappingContext, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/PJS/Inputs/IMC_Hunter.IMC_Hunter'"));

    // State
    CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

    // Movement
    CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");

    // Weapon
    CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");

}
