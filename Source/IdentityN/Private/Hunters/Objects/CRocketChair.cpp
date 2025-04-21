#include "Hunters/Objects/CRocketChair.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Hunters/Characters/CHunter.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Survivor/Characters/Survivor.h"

ACRocketChair::ACRocketChair()
{
	PrimaryActorTick.bCanEverTick = true;

    CHelpers::CreateComponent(this, &Box, "Box");
    Box->SetBoxExtent(FVector(100, 50, 150));

    CHelpers::CreateComponent(this, &Mesh, "Mesh", Box);
    Mesh->SetRelativeLocation(FVector(~49, 5, ~149));
    Mesh->SetRelativeScale3D(FVector(5, 7, 5));

    UStaticMesh* mesh;
    CHelpers::GetAsset<UStaticMesh>(&mesh, TEXT("/Script/Engine.StaticMesh'/Game/PJS/Objects/RocketChair/Meshes/RocketChair.RocketChair'"));
    Mesh->SetStaticMesh(mesh);

    Box->OnComponentBeginOverlap.AddDynamic(this, &ACRocketChair::OnComponentBeginOverlap);
    Box->OnComponentEndOverlap.AddDynamic(this, &ACRocketChair::OnComponentEndOverlap);

}

void ACRocketChair::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACRocketChair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (survivor)
    {
        survivor->SeatGauge += DeltaTime;

        CLog::Print(survivor->SeatGauge);
    }

}

void ACRocketChair::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ACHunter* hunter = Cast<ACHunter>(OtherActor))
    {
        ASurvivor* candidate = Cast<ASurvivor>(CHelpers::GetComponent<UCWeaponComponent>(hunter)->Candidate);
        CheckNull(candidate);

        survivor = candidate;
    }

}

void ACRocketChair::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ASurvivor* survivor = Cast<ASurvivor>(OtherActor))
    {
        if (survivor->SeatGauge < 50)
            survivor->SeatGauge = 0;

        survivor = nullptr;
    }

}
