#include "Hunters/Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

UCMovementComponent::UCMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    // IA_Movement
    CHelpers::GetAsset<UInputAction>(&IA_Movement, TEXT("/Script/EnhancedInput.InputAction'/Game/PJS/Inputs/IA_Hunter_Movement.IA_Hunter_Movement'"));

    // IA_Look
    CHelpers::GetAsset<UInputAction>(&IA_Look, TEXT("/Script/EnhancedInput.InputAction'/Game/PJS/Inputs/IA_Hunter_Look.IA_Hunter_Look'"));

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

    OnBaseWalk();

    DisableControlRotation();

}

void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCMovementComponent::OnBindEnhancedInputSystem(UEnhancedInputComponent* InEnhancedInput)
{
    // Movement
    InEnhancedInput->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMovement);

    // Look
    InEnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnLook);

}

void UCMovementComponent::SetSpeed(ESpeedType InSpeed)
{
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InSpeed];

}

void UCMovementComponent::OnBaseWalk()
{
    SetSpeed(ESpeedType::BaseWalk);

}

void UCMovementComponent::OnSpearfishing()
{
    SetSpeed(ESpeedType::Spearfishing);

}

void UCMovementComponent::EnableControlRotation()
{
    OwnerCharacter->bUseControllerRotationYaw = true;
    OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

}

void UCMovementComponent::DisableControlRotation()
{
    OwnerCharacter->bUseControllerRotationYaw = false;
    OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

}

void UCMovementComponent::OnMovable()
{
    bMovable = true;

}

void UCMovementComponent::OffMovable()
{
    bMovable = false;

}

void UCMovementComponent::OnMovement(const FInputActionValue& InVal)
{
    CheckFalse(bMovable);

    // 캐릭터의 정면 방향을 가져오기 위해 컨트롤러의 회전 값을 구해서 Z축만을 사용
    FRotator rot = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);

    // Forward
    OwnerCharacter->AddMovementInput(FQuat(rot).GetForwardVector(), InVal.Get<FVector2D>().X);

    // Right
    OwnerCharacter->AddMovementInput(FQuat(rot).GetRightVector(), InVal.Get<FVector2D>().Y);

}

void UCMovementComponent::OnLook(const FInputActionValue& InVal)
{
    CheckTrue(bFixedCamera);

    // Horizontal
    OwnerCharacter->AddControllerYawInput(InVal.Get<FVector2D>().X * HorizontalLook);

    // Vertical
    OwnerCharacter->AddControllerPitchInput(InVal.Get<FVector2D>().Y * VerticalLook);

}
