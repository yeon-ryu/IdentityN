#include "Hunters/Weapons/AddOns/CWaterpool.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Hunters/Characters/CHunter_Naiad.h"

ACWaterPool::ACWaterPool()
{
	PrimaryActorTick.bCanEverTick = true;

    CHelpers::CreateComponent<USphereComponent>(this, &Collision, "Collision");
    Collision->SetSphereRadius(70);

    UNiagaraSystem* effect;
    CHelpers::GetAsset<UNiagaraSystem>(&effect, TEXT("/Script/Niagara.NiagaraSystem'/Game/PJS/VFXs/Puddle/Effect_Systems/Pick_Up_10/NS_Puddle.NS_Puddle'"));

    CHelpers::CreateComponent<UNiagaraComponent>(this, &OpeningPoolEffect, "OpeningPoolEffect", Collision);
    OpeningPoolEffect->SetAsset(effect);
    OpeningPoolEffect->SetRelativeLocation(FVector(0, 0, 10));
    OpeningPoolEffect->SetRelativeScale3D(FVector(0.7));
    OpeningPoolEffect->bAutoActivate = false;

    CHelpers::CreateComponent<UNiagaraComponent>(this, &ClosingPoolEffect, "ClosingPoolEffect", Collision);
    ClosingPoolEffect->SetAsset(effect);
    ClosingPoolEffect->SetRelativeLocation(FVector(0, 0, 10));
    ClosingPoolEffect->SetRelativeScale3D(FVector(0.7));
    ClosingPoolEffect->bAutoActivate = false;
    ClosingPoolEffect->SetVisibility(false);

}

void ACWaterPool::BeginPlay()
{
	Super::BeginPlay();

    // Collision overlap 바인딩 등 포함
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ACWaterPool::OnComponentBeginOverlap);

    if (OpeningPoolEffect)
        OpeningPoolEffect->Activate(true);

    SetLifeSpan(InitialLifeSpan);

}

void ACWaterPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACWaterPool::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACWaterPool* pool = Cast<ACWaterPool>(OtherActor);
    if (!pool or pool == this) return;
    if (!OwnerCharacter || Index == -1 || pool->Index == -1) return;

    const int32 FromIndex = FMath::Min(Index, pool->Index);
    const int32 ToIndex = FMath::Max(Index, pool->Index);

    OwnerCharacter->ConvertPoolsToSwamp(FromIndex, ToIndex);

}

void ACWaterPool::InitializePool(ACHunter_Naiad* InOwner, int32 InIndex)
{
    OwnerCharacter = InOwner;
    Index = InIndex;

}

void ACWaterPool::TryFormSwampFromThis()
{
    if (bInSwamp) return;

    if (ConnectedPools.Num() < 2) return; // 자기 포함 최소 3개

    TSet<ACWaterPool*> AllConnected;
    CollectConnectedPoolsRecursive(this, AllConnected);

    if (AllConnected.Num() >= 3)
    {
        for (ACWaterPool* Pool : AllConnected)
        {
            if (!Pool->bInSwamp)
                Pool->ActivateSwampEffect();
        }
    }

}

void ACWaterPool::CollectConnectedPoolsRecursive(ACWaterPool* From, TSet<ACWaterPool*>& OutSet)
{
    if (!From || OutSet.Contains(From)) return;

    OutSet.Add(From);

    for (ACWaterPool* Neighbor : From->ConnectedPools)
        CollectConnectedPoolsRecursive(Neighbor, OutSet);
    
}

void ACWaterPool::ActivateSwampEffect()
{
    if (bInSwamp) return;

    bInSwamp = true;

    // 기존 이펙트를 숨기고 새로운 이펙트를 켜기
    if (OpeningPoolEffect->IsActive())
    {
        ClosingPoolEffect->SetVisibility(false);
        OpeningPoolEffect->Deactivate();
    }

    if (ClosingPoolEffect)
    {
        ClosingPoolEffect->SetVisibility(true);
        ClosingPoolEffect->Activate(true);
    }

    SetLifeSpan(ExtendedLifeSpan); // 생명 연장

}

void ACWaterPool::ExtendLifeSpan()
{
    // 한 번만 연장
    if (bLifeExtended)
        return;

    SetLifeSpan(ExtendedLifeSpan);
    bLifeExtended = true;

}
