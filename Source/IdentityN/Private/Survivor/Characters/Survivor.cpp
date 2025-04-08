// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Characters/Survivor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Survivor/Components/SMove.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "UObject/ConstructorHelpers.h"
#include "Survivor/Animations/SAnimInstance.h"

// Sets default values
ASurvivor::ASurvivor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    
    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character
    CameraBoom->SetRelativeLocation(FVector(0, 0, 40));
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->SetRelativeLocation(FVector(0, 0, 40));
    FollowCamera->bUsePawnControlRotation = false;

    MoveComp = CreateDefaultSubobject<USMove>(TEXT("MoveComp"));


    ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/RGY/Inputs/IMC_Survivor.IMC_Survivor'"));
    if (TempIMC.Succeeded()) {
        IMC_Survivor = TempIMC.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> TempIALook(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
    if (TempIALook.Succeeded()) {
        IA_Look = TempIALook.Object;
    }
}

// Called when the game starts or when spawned
void ASurvivor::BeginPlay()
{
	Super::BeginPlay();
	
    auto pc = Cast<APlayerController>(Controller);
    
    if (pc) {
        auto CameraManager = pc->PlayerCameraManager;
        CameraManager->ViewPitchMin = -15.0f;
        CameraManager->ViewPitchMax = 15.0f;
    }

    if(AnimInstance == nullptr) {
        AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    }
}

// Called every frame
void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        MoveComp->SetupInputBinding(input);

        // Looking
        input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASurvivor::Look);
    }
}

void ASurvivor::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(IMC_Survivor, 1);
        }
    }
}

float ASurvivor::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    HP -= Damage;

    return HP;
}

void ASurvivor::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

