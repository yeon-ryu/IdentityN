// Copyright Epic Games, Inc. All Rights Reserved.

#include "IdentityNGameMode.h"
#include "IdentityNCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Items/Door.h"
#include "EngineUtils.h"
#include "Utilities/CLog.h"
#include "Items/CipherMachine.h"

AIdentityNGameMode::AIdentityNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AIdentityNGameMode::BeginPlay()
{

}

void AIdentityNGameMode::AddDecodeCipher()
{
    if(decodeCipherCount >= 5) return;

    ++decodeCipherCount;

    CLog::Print(FString::Printf(TEXT("Remain Cipher : %d"), (5 - decodeCipherCount)), -1, 5.0f, FColor::Red);

    // 플레이어 UI 의 해독기 수 변화

    // 대문 열리는 조건 : 기본 모드
    if (decodeCipherCount >= 5) {
        // 전기 들어온 효과음

        // 모든 플레이어한테 알림 -> 생존자는 SBuff->CompleteDecode() 가 발동

        CLog::Print("Door is power on", -1, 5.0f, FColor::Red);

        // 맵의 모든 대문을 찾아 열릴 수 있도록 변경
        for (TActorIterator<ADoor> it(GetWorld()); it; ++it) {
            ADoor* door = *it;
            door->PowerOn();
        }

        // 맵의 모든 해독기를 정지시킨다.
        for (TActorIterator<ACipherMachine> it(GetWorld()); it; ++it) {
            ACipherMachine* machine = *it;
            machine->CipherComplete();
        }
    }
}

int32 AIdentityNGameMode::GetDecodeCipherCount()
{
    // 나중에 게임 UI 에서 남은 해독기 수 같은 것을 표시할 때 정보 사용

    return decodeCipherCount;
}
