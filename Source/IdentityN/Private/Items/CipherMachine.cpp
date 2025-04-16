// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CipherMachine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Survivor/Characters/Survivor.h"
#include "Survivor/Components/SInteractionItem.h"
#include "Utilities/CLog.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "IdentityNGameMode.h"

// Sets default values
ACipherMachine::ACipherMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    SetRootComponent(CollisionComp);
    CollisionComp->SetSphereRadius(63.0f);

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    // 암호기 에셋 추가
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionObjectType(ECC_WorldStatic);

    Antenna = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Antenna"));
    // 안테나 에셋 추가
    Antenna->SetupAttachment(RootComponent);
    Antenna->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACipherMachine::BeginPlay()
{
	Super::BeginPlay();
	
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACipherMachine::OnMachineOverlap);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ACipherMachine::OnMachineEndOverlap);
}

// Called every frame
void ACipherMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(State == EChiperState::COMPLETE) return;
    for (auto s : survivorList) {
        Decode(s);
    }
}

void ACipherMachine::StartDecode(class ASurvivor* survivor)
{
    if(survivor == nullptr || !survivor->InteractionItemComp->GetIsNearMachine() || State == EChiperState::COMPLETE) return;
    survivor->State = ESurvivorState::DECODE;
    survivor->AnimInstance->State = ESurvivorState::DECODE;
    survivorList.Add(survivor);
    
    if(State == EChiperState::READY) State = EChiperState::DECODEING;
    CLog::Print("Decode Start");
}

void ACipherMachine::Decode(class ASurvivor* survivor)
{
    if (survivor->State != ESurvivorState::DECODE) {
        RemoveSurvivor(survivor, true);
        return;
    }

    float addPer = GetWorld()->GetDeltaSeconds() / survivor->InteractionItemComp->GetDecodeTime();
    DecodeGauge += addPer;

    if (DecodeGauge >= 1.0f) {
        EndDecode();
    }
    else {
        // 타이머로 미니게임 부른다
    }

    CLog::Print(FString::Printf(TEXT("Decoding... %.2f"), DecodeGauge), -1, -1, FColor::Cyan);
}

void ACipherMachine::EndDecode()
{
    if (State == EChiperState::COMPLETE) return;
    State = EChiperState::COMPLETE;

    auto gm = Cast<AIdentityNGameMode>(GetWorld()->GetAuthGameMode());
    if (gm) {
        gm->AddDecodeCipher();
    }

    for (auto s : survivorList) {
        s->InteractionItemComp->OutCipherArea();
    }
    survivorList.Empty();

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CLog::Print("Decode End");
}

void ACipherMachine::RemoveSurvivor(class ASurvivor* survivor, bool bAuth /*= false*/)
{
    if (survivor == nullptr) return;

    if (bAuth) {
        // 다시 해독 시도할 수 있는 상태
        survivor->InteractionItemComp->EndDecode();
    }
    else {
        survivor->InteractionItemComp->OutCipherArea();
    }

    for (int i = 0; i < survivorList.Num(); i++) {
        if (survivorList[i]->GetPlayerIdx() == survivor->GetPlayerIdx()) {
            // 순서는 중요하지 않기에 RemoveAt 대신 사용
            survivorList.RemoveAtSwap(i);
            break;
        }
    }

    if (survivorList.Num() == 0 && DecodeGauge < 1.0f) {
        State = EChiperState::READY;
    }
}

void ACipherMachine::MiniGame()
{
    // 미니게임 발생 -> 성공/실패 처리는 생존자 로직에서
    // 실패하면 RemoveSurvivor(), DECODEFAIL -> IDLE 상태로
}

void ACipherMachine::CipherComplete()
{
    State = EChiperState::COMPLETE;
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (survivorList.Num() > 0) {
        for (auto s : survivorList) {
            s->InteractionItemComp->OutCipherArea();
        }
    }
}

float ACipherMachine::GetProgressRate()
{
    return DecodeGauge;
}

void ACipherMachine::OnMachineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto survivor = Cast<ASurvivor>(OtherActor);
    if (survivor == nullptr) return;

    survivor->InteractionItemComp->InCipherArea(this);
    CLog::Print(FString::Printf(TEXT("%d Survivor In the Decode Area"), survivor->GetPlayerIdx()));
}

void ACipherMachine::OnMachineEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    auto survivor = Cast<ASurvivor>(OtherActor);
    if (survivor == nullptr) return;

    bool bWorker = false;

    CLog::Print(FString::Printf(TEXT("%d Survivor Out of Decode Area"), survivor->GetPlayerIdx()));

    RemoveSurvivor(survivor);
}