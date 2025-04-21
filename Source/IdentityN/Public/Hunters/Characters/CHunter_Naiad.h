#pragma once

#include "CoreMinimal.h"
#include "Hunters/Characters/CHunter.h"
#include "CHunter_Naiad.generated.h"

UCLASS(Blueprintable)
class IDENTITYN_API ACHunter_Naiad : public ACHunter
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ACWaterPool> WaterPoolClass;
	
public:
    ACHunter_Naiad();

    virtual void PostInitializeComponents() override;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // 웅덩이가 Destroy될 때 호출되어 Map에서 제거
    UFUNCTION()
    void OnPoolDestroyed(AActor* InDestroyedActor);

public:
    // 물웅덩이를 생성하고 초기화 후 반환
    ACWaterPool* SpawnWaterpool();

    // 두 웅덩이 인덱스 사이에 있는 웅덩이들을 모두 습지로 전환
    void ConvertPoolsToSwamp(int32 FromIndex, int32 ToIndex);

private:
    // 고유 ID(PoolIndex)와 웅덩이 Actor를 매핑하는 맵
    UPROPERTY()
    TMap<int32, ACWaterPool*> ActivePools;

private:
    // 다음 생성될 웅덩이에 부여할 고유 ID
    int32 NextPoolID = 0;

};
