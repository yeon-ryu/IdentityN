#include "Survivor/Components/SBuff.h"
#include "Survivor/Characters/Survivor.h"
#include "Survivor/Components/SMove.h"

USBuff::USBuff()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USBuff::BeginPlay()
{
	Super::BeginPlay();

    me = Cast<ASurvivor>(GetOwner());
    if (me == nullptr) return;
	
    SetInitData();
}


void USBuff::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USBuff::DamagedBuff()
{
    // 피격시 이속 증가 버프
    // 기본적으로 5퍼센트만큼 2초간 이속 증가
    me->MoveComp->BuffSpeed(DamageSpeedPer, DamageSpeedSeconds);
    
}

void USBuff::CompleteDecode()
{
    // 마지막 희망 등 적용
    if (BorrowedTime) {
        // me 의 HP 가 0 일 경우, 플레이어의 HP 를 1로 회복하고 UnCrouch()
        // 치료 받는 로직에 같은 부분이 있을테니 해당 컴포넌트의 함수를 호출

    }


}

void USBuff::SetInitData()
{
    auto data = me->SurvivorData;

    if (data == nullptr) {
        // 값 못 가져왔을 때 디폴트 값
        DamageSpeedSeconds = 2.0f;
        return;
    }

    DamageSpeedSeconds = data->DamageSpeedTime;
}

