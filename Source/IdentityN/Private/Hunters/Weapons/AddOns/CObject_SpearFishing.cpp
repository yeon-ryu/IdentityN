#include "Hunters/Weapons/AddOns/CObject_SpearFishing.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Hunters/Weapons/CAttachment.h"

ACObject_SpearFishing::ACObject_SpearFishing()
{
    PrimaryActorTick.bCanEverTick = true;

    CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
    CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
    CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Root);

    CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

    Projectile->ProjectileGravityScale = 0;

}

void ACObject_SpearFishing::BeginPlay()
{
    Super::BeginPlay();

    Ignores.Add(GetOwner());

    Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //Capsule->OnComponentHit.AddDynamic(this, &ACObject_SpearFishing::OnComponentHit);
    Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACObject_SpearFishing::OnComponentBeginOverlap);

    Projectile->Velocity = GetOwner()->GetActorForwardVector() * Projectile->InitialSpeed;
    Projectile->bRotationFollowsVelocity = true;
    Projectile->Activate();

}

void ACObject_SpearFishing::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (OnEndPlay.IsBound())
        OnEndPlay.Broadcast(this);
    
}

void ACObject_SpearFishing::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bReturn)
    {
        Projectile->Velocity = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal() * Projectile->InitialSpeed;

        if (FVector::Dist(GetActorLocation(), GetOwner()->GetActorLocation()) <= 10)
        {
            UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(GetOwner());
            CheckNull(weapon);
            CheckNull(weapon->GetAttachment());
            weapon->GetAttachment()->SetActorHiddenInGame(false);
            weapon->bCanSkill = true;
            weapon->bUsedSkill = false;

            Destroy();
        }
    }

}

void ACObject_SpearFishing::Return(FVector InDestination)
{
    bReturn = true;

    SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InDestination));
    Projectile->Velocity = InDestination - GetActorLocation();
    Projectile->Activate();

}

void ACObject_SpearFishing::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //for (AActor* actor : Ignores)
    //    CheckTrue(actor == OtherActor);

    if (OtherActor)
    {
        Projectile->Deactivate();
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    //if (!!OtherActor and OnHit.IsBound())
    //    OnHit.Broadcast(this, OtherActor);

    //ACharacter* character = Cast<ACharacter>(OtherActor);
    //if (!!character && OnHit.IsBound())
    //    OnHit.Broadcast(this, character);

}

void ACObject_SpearFishing::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bReturn)
    {
        if (OtherActor != GetOwner()) return;

        Projectile->Deactivate();
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else
    {
        if (OtherActor == GetOwner()) return;

        Projectile->Deactivate();
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

}
