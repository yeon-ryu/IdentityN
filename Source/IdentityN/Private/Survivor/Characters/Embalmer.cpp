// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Characters/Embalmer.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Survivor/Animations/EmbalmerAnim.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AEmbalmer::AEmbalmer()
{
    // 캐릭터 식별자
    //Id = 1;
    Id = 2; // 테스트용 수치 가진 캐릭터 Id

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

    Bag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bag"));
    Bag->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
    Bag->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ConstructorHelpers::FObjectFinder<UStaticMesh>TempBagMesh (TEXT("/Script/Engine.StaticMesh'/Game/RGY/Modelings/embalmer/item/close_bag.close_bag'"));
    if (TempBagMesh.Succeeded()) {
        Bag->SetStaticMesh(TempBagMesh.Object);
        Bag->SetVisibility(false);
    }
}

void AEmbalmer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Bag) {
        // Tick 에서 해주기엔 과한 작업 아닌가 싶은데 Ember 가 상태 변환하는거 체크하려면 tick 에서 와야하니..
        if (State == ESurvivorState::IDLE && !bCrawl && !Bag->IsVisible()) {
            Bag->SetVisibility(true);
        }
        else if (Bag->IsVisible() && (State != ESurvivorState::IDLE || bCrawl || AnimInstance->falling)) {
            Bag->SetVisibility(false);
        }
    }
}

void AEmbalmer::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = Cast<UEmbalmerAnim>(GetMesh()->GetAnimInstance());
}