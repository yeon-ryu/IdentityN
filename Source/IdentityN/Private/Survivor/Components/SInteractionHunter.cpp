#include "Survivor/Components/SInteractionHunter.h"
#include "Survivor/Characters/Survivor.h"
#include "Hunters/Characters/CHunter.h"
#include "Utilities/CLog.h"
#include "Components/CapsuleComponent.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

USInteractionHunter::USInteractionHunter()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionHunter::BeginPlay()
{
	Super::BeginPlay();
	
    me = Cast<ASurvivor>(GetOwner());
    if (me == nullptr) return;
    
    me->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &USInteractionHunter::OnSenseOverlap);
    me->GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &USInteractionHunter::OnSenseEndOverlap);
}

void USInteractionHunter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bNearHunter) {
        CheckDistanceHunter();
    }

    if(CatchHunter != nullptr) {
        // 풍선 게이지 시간에 따라 생존자 데이터에 따라 차도록 로직 추가

        FRotator hRot = CatchHunter->GetActorRotation();
        me->SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(hRot.Yaw), 0.0f));

        if (BalloonGauge > 1.0f) {
            EscapeBallooned();
        }
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

    CLog::Print(FString::Printf(TEXT("Hunter is Near! : %.2f"), dis), -1, -1, FColor::Green);
}

void USInteractionHunter::CatchBallooned(ACHunter* hunter)
{
    CatchHunter = hunter;

    me->State = ESurvivorState::BALLOONED;
    me->AnimInstance->State = ESurvivorState::BALLOONED;

    BalloonGauge = 0.0f;
}

void USInteractionHunter::ReleaseBallooned()
{
    if(CatchHunter) {
        CatchHunter = nullptr;
    }

    me->State = ESurvivorState::IDLE;
    me->AnimInstance->State = ESurvivorState::IDLE;
}

void USInteractionHunter::EscapeBallooned()
{
    if(CatchHunter) {
        CatchHunter->EscapeSurvivor();
        CatchHunter = nullptr;
    }

    me->State = ESurvivorState::IDLE;
    me->AnimInstance->State = ESurvivorState::IDLE;
    me->bCrawl = false;
    me->SetHP(1.0f);

    BalloonGauge = 0.0f;
}

void USInteractionHunter::TryEscapeBallooned(float strength)
{
    BalloonGauge += strength;
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

