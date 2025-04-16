#include "Survivor/Components/SInteractionItem.h"
#include "Survivor/Characters/Survivor.h"
#include "Items/CipherMachine.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Items/Door.h"
#include "Kismet/KismetMathLibrary.h"

USInteractionItem::USInteractionItem()
{
	PrimaryComponentTick.bCanEverTick = true;

    ConstructorHelpers::FObjectFinder<UInputAction> TempIADecode(TEXT("/Script/EnhancedInput.InputAction'/Game/RGY/Inputs/IA_Decode.IA_Decode'"));
    if (TempIADecode.Succeeded()) {
        IA_Decode = TempIADecode.Object;
    }
}


void USInteractionItem::BeginPlay()
{
	Super::BeginPlay();

    me = Cast<ASurvivor>(GetOwner());
    if (me == nullptr) return;

    //SetInitData();
}


void USInteractionItem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USInteractionItem::SetupInputBinding(class UEnhancedInputComponent* input)
{
    input->BindAction(IA_Decode, ETriggerEvent::Started, this, &USInteractionItem::Decode);
}

void USInteractionItem::Decode(const struct FInputActionValue& value)
{
    // 다른 행동 중이면 해당 행동을 중지하기 전까지는 새로운 액션 불가
    if (me->State == ESurvivorState::OPEN || me->State == ESurvivorState::DECODE || me->State == ESurvivorState::DECODE_FAIL || me->bCrawl || me->AnimInstance->falling) return;

    // 1 순위 : 구출 가능한 (의자에 앉아있는) 생존자 (플레이어의 앞쪽에 있어야 함)
    
    // 2 순위 : 치료 가능한 (멈춰있는 다친) 생존자 (플레이어의 앞쪽에 있어야 함)

    // 3 순위 : 암호기나 문 해독 (방향은 중요하지 않음)
    if (bNearDoor) {
        if (NearDoor == nullptr) {
            OutDoorArea();
            return;
        }
        
        // 해독 / 오픈 하려는 물체 방향을 보도록
        FVector dir = NearDoor->GetPannelLocation() - me->GetActorLocation();
        me->SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(dir.GetSafeNormal().Rotation().Yaw), 0.0f));

        NearDoor->StartOpen(me);

    } else if(bNearChipher) {
        if (NearChipher == nullptr) {
            OutCipherArea();
            return;
        }

        FVector dir = NearChipher->GetActorLocation() - me->GetActorLocation();
        me->SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(dir.GetSafeNormal().Rotation().Yaw), 0.0f));

        NearChipher->StartDecode(me);
    }
}

void USInteractionItem::EndDecode()
{
    if(me->State == ESurvivorState::DECODE) {
        me->State = ESurvivorState::IDLE;
        me->AnimInstance->State = ESurvivorState::IDLE;
    }
}

void USInteractionItem::InCipherArea(class ACipherMachine* machine)
{
    bNearChipher = true;
    NearChipher = machine;
}

void USInteractionItem::OutCipherArea()
{
    EndDecode();
    bNearChipher = false;
    NearChipher = nullptr;
}

bool USInteractionItem::GetIsNearMachine()
{
    return bNearChipher;
}

float USInteractionItem::GetDecodeTime()
{
    return DecodeTime;
}

void USInteractionItem::SetDecodeTime(float time)
{
    DecodeTime = time;
}

bool USInteractionItem::GetIsNearDoor()
{
    return bNearDoor;
}

float USInteractionItem::GetOpenTime()
{
    return OpenDoorTime;
}

void USInteractionItem::EndOpen()
{
    if (me->State == ESurvivorState::OPEN) {
        me->State = ESurvivorState::IDLE;
        me->AnimInstance->State = ESurvivorState::IDLE;
    }
}

void USInteractionItem::InDoorArea(ADoor* door)
{
    bNearDoor = true;
    NearDoor = door;
}

void USInteractionItem::OutDoorArea()
{
    EndOpen();
    bNearDoor = false;
    NearDoor = nullptr;
}

void USInteractionItem::SetInitData()
{
    // 서버에 있는 csv 에서 읽어온 데이터 중 SurvivorId 로 데이터 가져와서 세팅
    auto data = me->SurvivorData;

    if (data) {
        DecodeTime = data->DecodeTime;
        DecodeFailPer = data->DecodeFailPer;
        DecodeFailTime = data->DecodeFailTime;
        OpenDoorTime = data->OpenDoorTime;
    }
}

