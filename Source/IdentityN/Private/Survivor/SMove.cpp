// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/SMove.h"
#include "Survivor/Survivor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "UObject/ConstructorHelpers.h"
#include "Survivor/SAnimInstance.h"

USMove::USMove()
{
    // 액터에 있으니 컴포넌트에는 없어도 되지 않나? -> Tick 안도는 문제 발생하면 주석 해제
	//PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void USMove::BeginPlay()
{
	Super::BeginPlay();

    me = Cast<ASurvivor>(GetOwner());
    if (me == nullptr) return;

    MoveComp = me->GetCharacterMovement();

    MoveComp->bOrientRotationToMovement = true;
    MoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 
    MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;

    MoveComp->MinAnalogWalkSpeed = 20.f;
    MoveComp->BrakingDecelerationWalking = 2000.f;
    MoveComp->BrakingDecelerationFalling = 1500.0f;

    SetMoveData();

    //ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_SMove.IA_SMove'"));
    //if (TempIAMove.Succeeded()) {
    //    IA_Move = TempIAMove.Object;
    //}

    //ConstructorHelpers::FObjectFinder<UInputAction> TempIACrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_Crouch.IA_Crouch'"));
    //if (TempIACrouch.Succeeded()) {
    //    IA_Crouch = TempIACrouch.Object;
    //}
}

void USMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USMove::SetupInputBinding(class UEnhancedInputComponent* input)
{
    input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &USMove::Move);
    input->BindAction(IA_Crouch, ETriggerEvent::Started, this, &USMove::CrouchToggle);
}

void USMove::Move(const struct FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (me->Controller != nullptr)
    {
        FVector dir = me->GetFollowCamera()->GetForwardVector() * MovementVector.X + me->GetFollowCamera()->GetRightVector() * MovementVector.Y;

        me->AddMovementInput(FVector(dir.X, dir.Y, 0.0f).GetSafeNormal());
    }
}

void USMove::CrouchToggle(const struct FInputActionValue& Value)
{
    if(me->bCrawl) return;

    bCrouch = !bCrouch;
    me->AnimInstance->bCrouch = bCrouch;

    if (bCrouch) {
        me->Crouch();
    }
    else {
        me->UnCrouch();
    }
}

void USMove::SetMoveData()
{
    // 서버에 있는 csv 에서 읽어온 데이터 중 PlayerId 로 데이터 가져와서 세팅


    // 값 못 가져왔을 때 디폴트 값
    MoveComp->MaxWalkSpeed = 380.f;
    MoveComp->MaxWalkSpeedCrouched = 114.f;
}

