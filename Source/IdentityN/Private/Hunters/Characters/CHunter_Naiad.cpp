#include "Hunters/Characters/CHunter_Naiad.h"
#include "Global.h"
#include "Hunters/Characters/CHunterAnimInstance.h"
#include "Hunters/Weapons/AddOns/CWaterPool.h"

ACHunter_Naiad::ACHunter_Naiad()
{
    PrimaryActorTick.bCanEverTick = true;

    HunterName = FName("Naiad");

}

void ACHunter_Naiad::PostInitializeComponents()
{
    Super::PostInitializeComponents();

}

void ACHunter_Naiad::BeginPlay()
{
    Super::BeginPlay();

    ActivePools.Empty();
    NextPoolID = 0;
    
}

void ACHunter_Naiad::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ACHunter_Naiad::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACHunter_Naiad::OnPoolDestroyed(AActor* InDestroyedActor)
{
    ACWaterPool* destroyed = Cast<ACWaterPool>(InDestroyedActor);
    if (!destroyed) return;

    for (auto it = ActivePools.CreateIterator(); it; ++it)
    {
        if (it.Value() == destroyed)
        {
            ActivePools.Remove(it.Key());

            break;
        }
    }

}

ACWaterPool* ACHunter_Naiad::SpawnWaterpool()
{
    CheckNullResult(WaterPoolClass, nullptr);

    if (ACWaterPool* pool = GetWorld()->SpawnActor<ACWaterPool>(WaterPoolClass, GetActorLocation(), FRotator::ZeroRotator))
    {
        const int32 id = ActivePools.Num();

        ActivePools.Add(id, pool);

        pool->InitializePool(this, id);
        pool->OnDestroyed.AddDynamic(this, &ACHunter_Naiad::OnPoolDestroyed);

        return pool;
    }

    return nullptr;
}

void ACHunter_Naiad::ConvertPoolsToSwamp(int32 FromIndex, int32 ToIndex)
{
    for (int32 i = FromIndex; i <= ToIndex; ++i)
    {
        ACWaterPool** found = ActivePools.Find(i);
        if (found and IsValid(*found))
        {
            (*found)->ActivateSwampEffect();

#if WITH_EDITOR
            if (i > FromIndex)
            {
                ACWaterPool** prev = ActivePools.Find(i - 1);
                if (prev and IsValid(*prev))
                {
                    FVector start = (*prev)->GetActorLocation();
                    FVector end = (*found)->GetActorLocation();
                    DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 5.f, 0, 2.f);
                }
            }
#endif
        }
    }

}
