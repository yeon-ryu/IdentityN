// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Embalmer.h"
#include "Survivor/SAnimInstance.h"
#include "Survivor/EmbalmerAnim.h"

AEmbalmer::AEmbalmer()
{
    PlayerId = 1;

    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/RGY/Modelings/embalmer/character-skeleton.character-skeleton'"));

    if (TempMesh.Succeeded()) {
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, 90.0f, 0.0f));
        GetMesh()->SetRelativeScale3D(FVector(4.0f));
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Embalmer SkeletalMesh loding fail."));
    }
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    ConstructorHelpers::FClassFinder<USAnimInstance> TempAnimInst(TEXT("/Script/Engine.AnimBlueprint'/Game/RGY/Blueprints/ABP_Embalmer.ABP_Embalmer_C'"));

    if (TempAnimInst.Succeeded()) {
        GetMesh()->SetAnimInstanceClass(TempAnimInst.Class);
    }
}

void AEmbalmer::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = Cast<UEmbalmerAnim>(GetMesh()->GetAnimInstance());
}
