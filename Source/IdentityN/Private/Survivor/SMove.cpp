// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/SMove.h"
#include "Survivor/Survivor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

// Sets default values for this component's properties
USMove::USMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

    MoveComp->MaxWalkSpeed = 500.f;
    MoveComp->MinAnalogWalkSpeed = 20.f;
    MoveComp->BrakingDecelerationWalking = 2000.f;
    MoveComp->BrakingDecelerationFalling = 1500.0f;
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
    input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USMove::Move);
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

        // add movement 
        me->AddMovementInput(FVector(MovementVector.Y, MovementVector.X, 0.0f).GetSafeNormal());
        //me->AddMovementInput(ForwardDirection, MovementVector.Y);
        //me->AddMovementInput(RightDirection, MovementVector.X);
    }
}

