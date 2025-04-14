// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Survivor/Characters/Survivor.h"
#include "Survivor/Components/SInteractionItem.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Utilities/CLog.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/BoxComponent.h"
#include "IdentityNGameMode.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    SetRootComponent(RootComp);

    // 문
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    ConstructorHelpers::FObjectFinder<UStaticMesh> TempDoorComp(TEXT("/Script/Engine.StaticMesh'/Game/RGY/Modelings/Door/double_door_metal/scene.scene'"));
    if (TempDoorComp.Succeeded()) {
        MeshComp->SetStaticMesh(TempDoorComp.Object);
        MeshComp->SetRelativeScale3D(FVector(1.5f));
        MeshComp->SetRelativeRotation(FRotator(0, 90, 0));
    }
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionObjectType(ECC_WorldStatic);

    // 문 조작 패널
    PannelComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PannelComp"));
    PannelComp->SetupAttachment(RootComponent);

    ConstructorHelpers::FObjectFinder<UStaticMesh> TempPannelComp(TEXT("/Script/Engine.StaticMesh'/Game/RGY/Modelings/Door/door-lock/source/WALL_THINGS_texturs.WALL_THINGS_texturs'"));
    if (TempPannelComp.Succeeded()) {
        PannelComp->SetStaticMesh(TempPannelComp.Object);
        PannelComp->SetRelativeScale3D(FVector(0.1f));
        PannelComp->SetRelativeLocationAndRotation(FVector(-22, 180, -30), FRotator(0, 180, 0));
    }
    PannelComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 패널 Collisions
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetupAttachment(PannelComp);
    CollisionComp->SetSphereRadius(25.0f);
    CollisionComp->SetRelativeScale3D(FVector(10));

    CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 탈출 Collision
    EscapeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EscapeComp"));
    EscapeCollision->SetupAttachment(RootComponent);
    EscapeCollision->SetRelativeLocation(FVector(300, 0, -110));
    EscapeCollision->SetRelativeScale3D(FVector(1.0f, 10.0f, 2.0f));

    EscapeCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    EscapeCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
    EscapeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 문 열기 애니메이션 타임라인
    OpenTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenTimeLineComp"));
    OpenCallback.BindUFunction(this, FName("OpenInterpReturn"));
    ConstructorHelpers::FObjectFinder<UCurveFloat> TempCurve(TEXT("/Script/Engine.CurveFloat'/Game/RGY/Blueprints/CV_OpenDoor.CV_OpenDoor'"));
    if (TempCurve.Succeeded()) {
        OpenCurve = TempCurve.Object;
    }
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnPannelOverlap);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnPannelEndOverlap);
    EscapeCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnEscapeOverlap);

    OpenTimeLineComp->SetLooping(false);
    OpenTimeLineComp->AddInterpFloat(OpenCurve, OpenCallback);
    OpenTimeLineComp->SetTimelineLength(TimeLineLength);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(survivor == nullptr || State == EDoorState::OPEN) return;
    Open();
}

void ADoor::PowerOn()
{
    // 대문 오픈 가능
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // 대문 패널 모든 맵에서 볼 수 있게 아웃라이너 빛나게
}

void ADoor::StartOpen(ASurvivor* sur)
{
    if (sur == nullptr || !sur->InteractionItemComp->GetIsNearDoor() || State == EDoorState::OPEN) return;
    sur->State = ESurvivorState::OPEN;
    sur->AnimInstance->State = ESurvivorState::OPEN;
    survivor = sur;

    CLog::Print("Open Door Start");
}

void ADoor::Open()
{
    if (survivor->State != ESurvivorState::OPEN) {
        survivor->InteractionItemComp->EndOpen();
        survivor = nullptr;
        return;
    }

    float addPer = GetWorld()->GetDeltaSeconds() / survivor->InteractionItemComp->GetOpenTime();
    OpenGauge += addPer;

    if (OpenGauge >= 1.0f) {
        EndOpen();
    }

    CLog::Print(FString::Printf(TEXT("Open... %.2f"), OpenGauge), -1, -1, FColor::Cyan);
}

void ADoor::EndOpen()
{
    if(State == EDoorState::OPEN) return;
    State = EDoorState::OPEN;

    RemoveSurvivor();

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CLog::Print("Open Door End");

    // 애니메이션으로 대문이 움직여서 열리게
    OpenTimeLineComp->PlayFromStart();

    // 탈출 가능
    EscapeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADoor::RemoveSurvivor()
{
    if(survivor == nullptr) return;

    survivor->InteractionItemComp->OutDoorArea();
    survivor = nullptr;
}

void ADoor::OnPannelOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto sur = Cast<ASurvivor>(OtherActor);
    if (sur == nullptr) return;

    sur->InteractionItemComp->InDoorArea(this);
    CLog::Print(FString::Printf(TEXT("%d Survivor In the Open Area"), sur->GetPlayerIdx()));
}

void ADoor::OnPannelEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    auto sur = Cast<ASurvivor>(OtherActor);
    if (sur == nullptr) return;

    CLog::Print(FString::Printf(TEXT("%d Survivor Out of Open Area"), sur->GetPlayerIdx()));

    RemoveSurvivor();
}

void ADoor::OnEscapeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto sur = Cast<ASurvivor>(OtherActor);
    if (sur == nullptr) return;

    auto gm = Cast<AIdentityNGameMode>(GetWorld()->GetAuthGameMode());
    gm->Escape(sur);
}

void ADoor::OpenInterpReturn(float value)
{
    MeshComp->SetRelativeLocation(FVector(0, value, 0));
}

