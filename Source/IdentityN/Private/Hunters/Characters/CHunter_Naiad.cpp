#include "Hunters/Characters/CHunter_Naiad.h"
#include "Global.h"
#include "Hunters/Characters/CHunterAnimInstance.h"

ACHunter_Naiad::ACHunter_Naiad()
{
    PrimaryActorTick.bCanEverTick = true;

    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, TEXT("/Script/Engine.SkeletalMesh'/Game/PJS/Characters/Naiad/Naiad.Naiad'"));
    CheckNull(mesh);
    GetMesh()->SetSkeletalMesh(mesh);

    TSubclassOf<UCHunterAnimInstance> anim;
    CHelpers::GetClass<UCHunterAnimInstance>(&anim, TEXT("/Script/Engine.AnimBlueprint'/Game/PJS/Characters/ABP_CHunterAnimInstance.ABP_CHunterAnimInstance_C'"));
    GetMesh()->SetAnimClass(anim);

}

void ACHunter_Naiad::BeginPlay()
{
    Super::BeginPlay();

}

void ACHunter_Naiad::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ACHunter_Naiad::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}
