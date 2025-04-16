// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8 {
    CLOSE UMETA(DisplayName = "닫힘"),
    OPEN UMETA(DisplayName = "열림")
};

UCLASS()
class IDENTITYN_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class ASurvivor* survivor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDoorState State = EDoorState::CLOSE;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class USceneComponent* RootComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* MeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* PannelComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class USphereComponent* CollisionComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* EscapeCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    float OpenGauge = 0.0f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OPEN, meta = (AllowPrivateAccess = "true"))
    class UTimelineComponent* OpenTimeLineComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OPEN, meta = (AllowPrivateAccess = "true"))
    class UCurveFloat* OpenCurve;

    FOnTimelineFloat OpenCallback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OPEN, meta = (AllowPrivateAccess = "true"))
    float TimeLineLength = 3.0f;


public:
    void PowerOn();

    void StartOpen(class ASurvivor* sur);

    void Open();

    void EndOpen();

    void RemoveSurvivor();

    FVector GetPannelLocation();

    float GetProgressRate();

private:
    UFUNCTION()
	void OnPannelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	void OnPannelEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
	void OnEscapeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


    UFUNCTION()
    void OpenInterpReturn(float value);
};
