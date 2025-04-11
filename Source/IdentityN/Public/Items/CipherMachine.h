// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CipherMachine.generated.h"

// 대기 중은 안테나 빛남, 해독중은 안테나 흔들림, 해독완료는 안테나 빛 사라짐
UENUM(BlueprintType)
enum class EChiperState : uint8 {
    READY UMETA(DisplayName = "대기"),
    DECODEING UMETA(DisplayName = "해독중"),
    COMPLETE UMETA(DisplayName = "해독완료")
};

UCLASS()
class IDENTITYN_API ACipherMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACipherMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<class ASurvivor*> survivorList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EChiperState State = EChiperState::READY;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class USphereComponent* CollisionComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* MeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* Antenna;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    float DecodeGauge = 0.0f;

public:
    void StartDecode(class ASurvivor* survivor);

    void Decode(class ASurvivor* survivor);

    void EndDecode();

    void RemoveSurvivor(class ASurvivor* survivor, bool bAuth = false);

    void MiniGame();

    void CipherComplete();

private:
    // 오버랩 이벤트로 들어올 때 서바이버의 bool 을 true 로 해주고
    // end 오버랩 이벤트 때 Remove 서바이버 해줌

    UFUNCTION()
	void OnMachineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	void OnMachineEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
