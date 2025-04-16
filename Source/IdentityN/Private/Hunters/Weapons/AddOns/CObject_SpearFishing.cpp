#include "Hunters/Weapons/AddOns/CObject_SpearFishing.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hunters/Components/CWeaponComponent.h"
#include "Hunters/Weapons/CAttachment.h"
#include "Engine/DamageEvents.h"

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

    if (!bReturn) return;

    UpdateReturnMovement();

    if (HasReachedOwner())
        CompleteReturnToOwner();

}

void ACObject_SpearFishing::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bReturn) HandleReturnOverlap(OtherActor);
    else HandleAttackOverlap(OtherActor);

    if (bReturn)
    {
        if (OtherActor != GetOwner()) return;

        Projectile->Deactivate();
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    else
    {
        if (OtherActor == GetOwner()) return;

        if (ACharacter* character = Cast<ACharacter>(OtherActor))
        {
            character->TakeDamage(1, FDamageEvent(), GetOwner()->GetInstigatorController(), this);
            return;
        }

        Projectile->Deactivate();
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

}

// 창의 반환 동작 초기화
void ACObject_SpearFishing::Return(FVector InDestination)
{
    bReturn = true;

    // 반환 방향으로 회전 설정
    SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InDestination));

    // 반환 속도 초기화 (속도 벡터 = 목적지 방향 벡터)
    FVector direction = (InDestination - GetActorLocation()).GetSafeNormal();
    Projectile->Velocity = direction * Projectile->InitialSpeed;

    // 발사체 활성화
    Projectile->Activate();

}

// 매 프레임마다 되돌아오는 방향으로 이동 처리
void ACObject_SpearFishing::UpdateReturnMovement()
{
    FVector ownerLocation = GetOwner()->GetActorLocation();
    FVector direction = (ownerLocation - GetActorLocation()).GetSafeNormal();

    Projectile->Velocity = direction * Projectile->InitialSpeed;

}

// 반환 목적지(플레이어)와 충분히 가까워졌는지 판단
bool ACObject_SpearFishing::HasReachedOwner() const
{
    constexpr float reachThreshold = 50.0f;
    float distance = FVector::Dist(GetActorLocation(), GetOwner()->GetActorLocation());

    return distance <= reachThreshold;

}

void ACObject_SpearFishing::CompleteReturnToOwner()
{
    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(GetOwner());
    CheckNull(weapon);

    AActor* attachment = weapon->GetAttachment();
    CheckNull(attachment);

    attachment->SetActorHiddenInGame(false);
    weapon->bCanSkill = true;
    weapon->bUsedSkill = false;

    Destroy();

}

// 되돌아오는 상태에서의 충돌 처리
void ACObject_SpearFishing::HandleReturnOverlap(AActor* InOtherActor)
{
    if (InOtherActor != GetOwner())
        return;

    DisableSpearCollision();

}

// 공격 상태에서의 충돌 처리
void ACObject_SpearFishing::HandleAttackOverlap(AActor* InOtherActor)
{
    if (InOtherActor == GetOwner())
        return;

    if (ACharacter* character = Cast<ACharacter>(InOtherActor))
    {
        constexpr int32 BaseDamage = 1;
        character->TakeDamage(BaseDamage, FDamageEvent(), GetOwner()->GetInstigatorController(), this);
    }

    DisableSpearCollision();

}

// 창의 충돌 비활성화 처리
void ACObject_SpearFishing::DisableSpearCollision()
{
    Projectile->Deactivate();
    Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
