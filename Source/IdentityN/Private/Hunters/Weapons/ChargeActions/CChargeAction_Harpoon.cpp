﻿#include "Hunters/Weapons/ChargeActions/CChargeAction_Harpoon.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Hunters/Components/CStateComponent.h"
#include "Hunters/Components/CMovementComponent.h"
#include "Hunters/Weapons/CAttachment.h"
#include "Hunters/Weapons/CDoAction.h"

void UCChargeAction_Harpoon::Pressed()
{
    CheckFalse(State->IsDefaultMode());
    CheckTrue(State->IsChargeActionMode());

    Super::Pressed();

    State->SetActionMode();
    State->OnChargeActionMode();

    ActionData.DoAction(Owner);

}

void UCChargeAction_Harpoon::Begin_ChargeAction_Implementation()
{
    Super::Begin_ChargeAction_Implementation();

    Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCChargeAction_Harpoon::OnAttachmentBeginOverlap);

    bMoving = true;

    Start = Owner->GetActorLocation();
    End = Start + Owner->GetActorForwardVector() * Distance;

    float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
    float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    FRotator rotation = Owner->GetActorRotation();


    TArray<AActor*> ignores;
    ignores.Add(Owner);

    TArray<FHitResult> hitResults;
    TArray<TEnumAsByte<EObjectTypeQuery>> objects;
    objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

    UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height), rotation, objects, false, ignores, DrawDebug, hitResults, true);
    for (const FHitResult& hitResult : hitResults)
    {
        ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

        if (!!character)
        {
            character->GetCapsuleComponent()->SetCollisionProfileName("Sword_ChargeAction");

            Overlapped.Add(character);
        }
    }

    FHitResult lineHitResult;
    UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DrawDebug, lineHitResult, true);

    if (lineHitResult.bBlockingHit)
    {
        FVector direction = (End - Start).GetSafeNormal2D();
        End = lineHitResult.Location - (direction * radius * 2);

        if (DrawDebug == EDrawDebugTrace::ForDuration)
            DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, true, 2);
    }

    if (DrawDebug == EDrawDebugTrace::ForDuration)
        DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);

}

void UCChargeAction_Harpoon::End_ChargeAction_Implementation()
{
    Super::End_ChargeAction_Implementation();

    Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
    Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

    bMoving = false;

    State->SetDefaultMode();
    State->OffChargeActionMode();

    Movement->OnMovable();
    Movement->DisableFixedCamera();

    for (ACharacter* character : Overlapped)
        character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");

    Overlapped.Empty();
    Hitted.Empty();

}

void UCChargeAction_Harpoon::Tick_Implementation(float InDeltaTime)
{
    Super::Tick_Implementation(InDeltaTime);
    CheckFalse(bMoving);

    FVector location = Owner->GetActorLocation();
    float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

    if (location.Equals(End, radius))
    {
        bMoving = false;
        Start = End = Owner->GetActorLocation();

        return;
    }

    FVector direction = (End - Start).GetSafeNormal2D();
    Owner->AddActorWorldOffset(direction * Speed, true);

}

void UCChargeAction_Harpoon::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther)
{
    CheckNull(InOther);

    for (ACharacter* character : Hitted)
        CheckTrue(character == InOther);

    Hitted.AddUnique(InOther);

    HitData.SendDamage(Owner, InAttackCuaser, InOther);

}
