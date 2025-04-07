// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/SMove.h"
#include "Survivor/Survivor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
USMove::USMove()
{
    // 액터에 있으니 컴포넌트에는 없어도 되지 않나? -> Tick 안도는 문제 발생하면 주석 해제
	//PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void USMove::BeginPlay()
{
	Super::BeginPlay();

	// ...
    me = Cast<ASurvivor>(GetOwner());
    if (me == nullptr) return;

    MoveComp = me->GetCharacterMovement();

    // Configure character movement
    MoveComp->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    MoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
    MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;

    MoveComp->MinAnalogWalkSpeed = 20.f;
    MoveComp->BrakingDecelerationWalking = 2000.f;
    MoveComp->BrakingDecelerationFalling = 1500.0f;

    SetMoveData();

    ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
    if (TempIAMove.Succeeded()) {
        IA_Move = TempIAMove.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> TempIACrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_Crouch.IA_Crouch'"));
    if (TempIACrouch.Succeeded()) {
        IA_Crouch = TempIACrouch.Object;
    }
}


// Called every frame
void USMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USMove::SetupInputBinding(class UEnhancedInputComponent* input)
{
    // Moving
    input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &USMove::Move);
}

void USMove::Move(const struct FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (me->Controller != nullptr)
    {
        // find out which way is forward
        //const FRotator Rotation = me->Controller->GetControlRotation();
        //const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        //const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // get right vector 
        //const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        FVector dir = me->GetFollowCamera()->GetForwardVector() * MovementVector.X + me->GetFollowCamera()->GetRightVector() * MovementVector.Y;

        // add movement 
        me->AddMovementInput(FVector(dir.X, dir.Y, 0.0f).GetSafeNormal());
        //me->AddMovementInput(ForwardDirection, MovementVector.Y);
        //me->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void USMove::SetMoveData()
{
    // 서버에 있는 csv 에서 읽어온 데이터 중 PlayerId 로 데이터 가져와서 세팅


    // 값 못 가져왔을 때 디폴트 값
    MoveComp->MaxWalkSpeed = 380.f;
    MoveComp->MaxWalkSpeedCrouched = 114.f;
}

