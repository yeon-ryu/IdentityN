#include "Survivor/Components/SInteractionItem.h"
#include "Survivor/Characters/Survivor.h"
#include "Items/CipherMachine.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Items/Door.h"

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

    SetInitData();
	
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
    if (me->State == ESurvivorState::OPEN || me->State == ESurvivorState::DECODE || me->State == ESurvivorState::DECODE_FAIL) return;

    if (bNearDoor) {
        if (NearDoor == nullptr) {
            EndOpen();
            return;
        }
        
        NearDoor->StartOpen(me);

    } else if(bNearChipher) {
        if (NearChipher == nullptr) {
            EndDecode();
            return;
        }

        NearChipher->StartDecode(me);
    }
}

void USInteractionItem::EndDecode()
{
    if(me->State == ESurvivorState::DECODE) {
        me->State = ESurvivorState::IDLE;
        me->AnimInstance->State = ESurvivorState::IDLE;
    }
    bNearChipher = false;
    NearChipher = nullptr;
}

void USInteractionItem::InCipherArea(class ACipherMachine* machine)
{
    bNearChipher = true;
    NearChipher = machine;
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
    bNearDoor = false;
    NearDoor = nullptr;
}

void USInteractionItem::InDoorArea(ADoor* door)
{
    bNearDoor = true;
    NearDoor = door;
}

void USInteractionItem::SetInitData()
{
    // 서버에 있는 csv 에서 읽어온 데이터 중 SurvivorId 로 데이터 가져와서 세팅
    auto data = me->SurvivorData;

    if (data == nullptr) {
        // 값 못 가져왔을 때 디폴트 값
        DecodeTime = 5.0f; // 81.0f;
        DecodeFailPer = 1.0f;
        DecodeFailTime = 2.0f;
        OpenDoorTime = 18.0f;
        return;
    }
}

