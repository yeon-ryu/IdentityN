#include "Hunters/Characters/CHunter.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

void ACHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Target)
        CLog::Print(Target->GetName());
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

void ACHunter::InitializeCharacters()
{
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

    // Fear
    CHelpers::CreateComponent<USphereComponent>(this, &Fear, "Fear", RootComponent);
    Fear->SetSphereRadius(3207);
    Fear->OnComponentBeginOverlap.AddDynamic(this, &ACHunter::OnComponentBeginOverlap);
    Fear->OnComponentEndOverlap.AddDynamic(this, &ACHunter::OnComponentEndOverlap);

    // Mapping Context
    CHelpers::GetAsset<UInputMappingContext>(&MappingContext, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/PJS/Inputs/IMC_Hunter.IMC_Hunter'"));

    // State
    CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

    // Movement
    CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");

    // Weapon
    CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");

}

void ACHunter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ACharacter* character = Cast<ACharacter>(OtherActor))
    {
        if (character == this) return;

        Target = character;

        CLog::Print(OtherActor->GetName());

        CLog::Print("In The Radius Of Fear");
    }

}

void ACHunter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Target == Cast<ACharacter>(OtherActor))
    {
        CLog::Print(OtherActor->GetName());

        CLog::Print("Out The Radius Of Fear");

        Target = nullptr;
    }

}
