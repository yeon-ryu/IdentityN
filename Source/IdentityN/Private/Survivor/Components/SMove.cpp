// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Components/SMove.h"
#include "Survivor/Characters/Survivor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "UObject/ConstructorHelpers.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Survivor/Data/SurvivorStruct.h"
#include "Utilities/CLog.h"
#include "Engine/TimerHandle.h"

USMove::USMove()
{
	PrimaryComponentTick.bCanEverTick = true;

    ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_SMove.IA_SMove'"));
    if (TempIAMove.Succeeded()) {
        IA_Move = TempIAMove.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> TempIACrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_Crouch.IA_Crouch'"));
    if (TempIACrouch.Succeeded()) {
        IA_Crouch = TempIACrouch.Object;
    }
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

    //SetInitData();
}

void USMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(me->State != ESurvivorState::IDLE) return;

    if (!bCrawl && me->bCrawl) {
        ChangeCrawl(true);
    }
    else if (bCrawl && !me->bCrawl) {
        ChangeCrawl(false);
    }
}

void USMove::SetupInputBinding(class UEnhancedInputComponent* input)
{
    input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &USMove::Move);
    input->BindAction(IA_Crouch, ETriggerEvent::Started, this, &USMove::CrouchToggle);
}

void USMove::Move(const struct FInputActionValue& Value)
{
    if (me->Controller == nullptr || me->IsOutofGame()) return;

    if (me->State == ESurvivorState::BALLOONED) {
        me->TryEscapeBallooned();
        return;
    }

    if(me->State == ESurvivorState::DECODE || me->State == ESurvivorState::OPEN || me->State == ESurvivorState::HEAL || me->State == ESurvivorState::RESCUE) {
        me->State = ESurvivorState::IDLE;
        me->AnimInstance->State = ESurvivorState::IDLE;
    }

    FVector2D MovementVector = Value.Get<FVector2D>();

    FVector dir = me->GetFollowCamera()->GetForwardVector() * MovementVector.X + me->GetFollowCamera()->GetRightVector() * MovementVector.Y;

    me->AddMovementInput(FVector(dir.X, dir.Y, 0.0f).GetSafeNormal());
}

void USMove::CrouchToggle(const struct FInputActionValue& Value)
{
    if(me->bCrawl) return;

    if (me->Controller == nullptr || me->IsOutofGame()) return;

    if (me->State == ESurvivorState::DECODE || me->State == ESurvivorState::OPEN || me->State == ESurvivorState::HEAL || me->State == ESurvivorState::RESCUE) {
        me->State = ESurvivorState::IDLE;
        me->AnimInstance->State = ESurvivorState::IDLE;
    }

    ChangeCrouch(!bCrouch);
}

void USMove::ChangeCrouch(bool crouchState)
{
    bCrouch = crouchState;
    me->AnimInstance->bCrouch = crouchState;

    if (crouchState) {
        me->Crouch();
    }
    else {
        me->UnCrouch();
    }
}

void USMove::ChangeCrawl(bool crawlState)
{
    bCrawl = crawlState;
    me->AnimInstance->bCrawl = crawlState;
    if(crawlState) {
        MoveComp->MaxWalkSpeedCrouched = crawlSpeed;
        me->Crouch();
    }
    else {
        MoveComp->MaxWalkSpeedCrouched = crouchSpeed;
        me->UnCrouch();
    }
}

void USMove::ResetSpeed()
{
    // SMove 에 걸린 타이머 전부 종료 (버프들 타이머) 처리
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

    MoveComp->MaxWalkSpeed = runSpeed;
    if (me->bCrawl) {
        MoveComp->MaxWalkSpeedCrouched = crawlSpeed;
    }
    else {
        MoveComp->MaxWalkSpeedCrouched = crouchSpeed;
    }
}

void USMove::BuffSpeed(float per, int seconds)
{
    float diff = 1.0f + per / 100.0f;
    MoveComp->MaxWalkSpeed = MoveComp->MaxWalkSpeed * diff;

    // 타이머 seconds 시간만큼 돌리고 종료되면 곱한만큼 나눠준다 -> 버프 중첩 대비
    FTimerHandle handle;
    GetWorld()->GetTimerManager().SetTimer(handle,
        FTimerDelegate::CreateLambda([&] {
            MoveComp->MaxWalkSpeed = MoveComp->MaxWalkSpeed / diff;
        }), seconds, false
    );
}

void USMove::SetInitData()
{
    // 서버에 있는 csv 에서 읽어온 데이터 중 SurvivorId 로 데이터 가져와서 세팅
    auto data = me->SurvivorData;
    
    if(data) {
        runSpeed = data->RunSpeed;
        crouchSpeed = data->CrouchSpeed;
        crawlSpeed = data->CrawlSpeed;
    }
    MoveComp->MaxWalkSpeed = runSpeed;
    MoveComp->MaxWalkSpeedCrouched = crouchSpeed;
}