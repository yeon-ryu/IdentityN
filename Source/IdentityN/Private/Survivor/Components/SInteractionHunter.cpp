#include "Survivor/Components/SInteractionHunter.h"
#include "Survivor/Characters/Survivor.h"
#include "Hunters/Characters/CHunter.h"
#include "Utilities/CLog.h"

USInteractionHunter::USInteractionHunter()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionHunter::BeginPlay()
{
	Super::BeginPlay();
	
}

void USInteractionHunter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bNearHunter) {
        CheckDistanceHunter();
    }
}

void USInteractionHunter::CheckDistanceHunter()
{
    if (me->IsOutofGame() || me->State == ESurvivorState::BALLOONED || me->State == ESurvivorState::SEAT) {
        return;
    }

    if (NearHunter == nullptr) {
        bNearHunter = false;
        return;
    }

    // 감시자가 하나만 존재한다고 가정하고 코드를 짬

    FVector loc = NearHunter->GetActorLocation() - me->GetActorLocation();
    // 인격 소름이 활성화 되있으면 이 정보를 활용해서 UI 에 감시자의 위치도 화살표로 표시됨

    // 생존자 자신의 UI 에만 표시
    // 감시자의 거리에 따라 UI 에 심장이 뚜렷하게 나타나고 거리가 멀어지면 심장이 연해진다.
    float dis = loc.Length();

    CLog::Print(FString::Printf(TEXT("Hunter is Near! : %.2f"), dis));
}

void USInteractionHunter::OnSenseOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto hunter = Cast<ACHunter>(OtherActor);
    if (hunter) {
        bNearHunter = true;
        NearHunter = hunter;
    }
}

void USInteractionHunter::OnSenseEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    auto hunter = Cast<ACHunter>(OtherActor);
    if (hunter) {
        bNearHunter = false;
        NearHunter = nullptr;
    }
}

