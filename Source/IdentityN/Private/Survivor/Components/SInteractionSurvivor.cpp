// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Components/SInteractionSurvivor.h"
#include "Survivor/Characters/Survivor.h"
#include "Survivor/Data/SurvivorEnum.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedPlayerInput.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CLog.h"

// Sets default values for this component's properties
USInteractionSurvivor::USInteractionSurvivor()
{
	PrimaryComponentTick.bCanEverTick = true;

    ConstructorHelpers::FObjectFinder<UInputAction> TempIAHeal(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_SHeal.IA_SHeal'"));
    if (TempIAHeal.Succeeded()) {
        IA_Heal = TempIAHeal.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> TempIASpace(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_SSpace.IA_SSpace'"));
    if (TempIASpace.Succeeded()) {
        IA_Space = TempIASpace.Object;
    }
}

void USInteractionSurvivor::BeginPlay()
{
	Super::BeginPlay();

    me = Cast<ASurvivor>(GetOwner());
    if (me == nullptr) return;
    
    me->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &USInteractionSurvivor::OnHealSearchOverlap);
    me->GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &USInteractionSurvivor::OnHealSearchEndOverlap);
}


// Called every frame
void USInteractionSurvivor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bHealing) {
        Heal();
    } else if (!bHurt && me->GetHP() <= 1.0f) {
        me->HealCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        bHurt = true;
        me->AnimInstance->bHurt = true;
    }
}

void USInteractionSurvivor::SetupInputBinding(class UEnhancedInputComponent* input)
{
    input->BindAction(IA_Heal, ETriggerEvent::Started, this, &USInteractionSurvivor::HealAction);
    input->BindAction(IA_Space, ETriggerEvent::Triggered, this, &USInteractionSurvivor::HealMyselfAction);
}

void USInteractionSurvivor::SetInitData()
{
    // 서버에 있는 csv 에서 읽어온 데이터 중 SurvivorId 로 데이터 가져와서 세팅
    auto data = me->SurvivorData;

    if (data) {
        HealTime = data->HealTime;
        HealSelfTime = data->HealSelfTime;
    }
}

// 힐 해주는 주체
void USInteractionSurvivor::StartHeal(ASurvivor* survivor)
{
    if (me->State == ESurvivorState::HEAL || me->State == ESurvivorState::HEAL_RECEIVE
        || survivor->State == ESurvivorState::HEAL || survivor->State == ESurvivorState::HEAL_RECEIVE
        || survivor->GetHP() > 1.0f) {
        return;
    }
    
    if (survivor->State == ESurvivorState::IDLE && survivor->AnimInstance->Speed == 0.0f) {
        FVector dir = survivor->GetActorLocation() - me->GetActorLocation();
        me->SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(dir.GetSafeNormal().Rotation().Yaw), 0.0f));

        survivor->InteractionSurvivorComp->bHealing = true;
        survivor->InteractionSurvivorComp->HealSurvivor = me;
        me->State = ESurvivorState::HEAL;
        me->AnimInstance->State = ESurvivorState::HEAL;
        survivor->State = ESurvivorState::HEAL_RECEIVE;
        survivor->AnimInstance->State = ESurvivorState::HEAL_RECEIVE;

        survivor->HealCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

// 힐 받는 주체
void USInteractionSurvivor::Heal()
{
    if (me->State == ESurvivorState::HEAL_RECEIVE) {
        if (HealSurvivor->State == ESurvivorState::HEAL) {
            HealGauge += GetWorld()->GetDeltaSeconds() / HealTime;

            if (HealGauge >= 1.0f) {
                EndHeal();
            }

            CLog::Print(FString::Printf(TEXT("Healing... %.2f"), HealGauge), -1, -1, FColor::Green);
        }
        else {
            bHealing = false;
            me->State = ESurvivorState::IDLE;
            me->AnimInstance->State = ESurvivorState::IDLE;
            HealSurvivor = nullptr;
            me->HealCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
    } else {
        HealSurvivor->State = ESurvivorState::IDLE;
        HealSurvivor->AnimInstance->State = ESurvivorState::IDLE;
        HealSurvivor = nullptr;
        me->HealCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

void USInteractionSurvivor::HealMyself()
{
    // 이건 스페이스 바 계속 누르고 있어야 그동안 자가 치유 된다. 속도는 HealSelfTime 에 근거한다
}

// 힐 받는 주체
void USInteractionSurvivor::EndHeal()
{
    bHealing = false;
    me->AddHP(1.0f);
    me->bCrawl = false;
    HealGauge = 0.0f;

    me->State = ESurvivorState::IDLE;
    me->AnimInstance->State = ESurvivorState::IDLE;

    HealSurvivor->State = ESurvivorState::IDLE;
    HealSurvivor->AnimInstance->State = ESurvivorState::IDLE;

    if (me->GetHP() > 1.0f) {
        HealSurvivor->InteractionSurvivorComp->NearSurvivor = nullptr;
        me->HealCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        bHurt = false;
        me->AnimInstance->bHurt = false;
    }
    else {
        me->HealCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    me->DeadGauge = 0.0f;
    HealSurvivor->InteractionSurvivorComp->HealSurvivor = nullptr;
    HealSurvivor = nullptr;
}

void USInteractionSurvivor::RemoveHealer()
{
    if(HealSurvivor == nullptr) return;

    HealSurvivor->State = ESurvivorState::IDLE;
    HealSurvivor->AnimInstance->State = ESurvivorState::IDLE;
    HealSurvivor->InteractionSurvivorComp->NearSurvivor = nullptr;
}

void USInteractionSurvivor::HealAction(const FInputActionValue& value)
{
    if(NearSurvivor == nullptr || bHealing || me->bCrawl) return;
    StartHeal(NearSurvivor);
}

void USInteractionSurvivor::HealMyselfAction(const FInputActionValue& value)
{
    if(bHealing || !me->bCrawl) return;
    HealMyself();
}

// 힐 해주는 주체
void USInteractionSurvivor::OnHealSearchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto survivor = Cast<ASurvivor>(OtherActor);
    if (survivor == nullptr || !survivor->InteractionSurvivorComp->bHurt) return;

    if (NearSurvivor == nullptr) {
        NearSurvivor = survivor;
        CLog::Print("Need Heal in range.");
    }
    else {
        auto distA = FVector::Distance(NearSurvivor->GetActorLocation(), me->GetActorLocation());
        auto distB = FVector::Distance(survivor->GetActorLocation(), me->GetActorLocation());

        if (distB < distA) {
            NearSurvivor = survivor;
            CLog::Print("Need Heal in range.");
        }
    }
}

// 힐 해주는 주체
void USInteractionSurvivor::OnHealSearchEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    auto survivor = Cast<ASurvivor>(OtherActor);
    if (survivor == nullptr || NearSurvivor == nullptr) return;

    if (NearSurvivor->GetPlayerIdx() == survivor->GetPlayerIdx()) {
        NearSurvivor = nullptr;
        CLog::Print("Need Heal out of range.");
    }
}

