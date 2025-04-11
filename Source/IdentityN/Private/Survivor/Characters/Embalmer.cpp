// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Characters/Embalmer.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Survivor/Animations/EmbalmerAnim.h"

AEmbalmer::AEmbalmer()
{
    // 캐릭터 식별자
    Id = 1;
    Name = "Embalmer"; // 이 라인 csv 데이터 추가 후 삭제

    // 캐릭터 메시
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/RGY/Modelings/embalmer/embalmer.embalmer'"));

    if (TempMesh.Succeeded()) {
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, 90.0f, 0.0f));
        GetMesh()->SetRelativeScale3D(FVector(1.6f));
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Embalmer SkeletalMesh loding fail."));
    }
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 캐릭터 전용 애니메이션
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