// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor/Characters/Survivor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Survivor/Components/SMove.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "UObject/ConstructorHelpers.h"
#include "Survivor/Animations/SAnimInstance.h"
#include "Utilities/CLog.h"
#include "Survivor/Components/SBuff.h"
#include "Survivor/Components/SInteractionItem.h"
#include "IdentityNGameInstance.h"
#include "IdentityNGameMode.h"

// Sets default values
ASurvivor::ASurvivor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    
    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
    CameraBoom->SetRelativeLocation(FVector(0, 0, 40));
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->SetRelativeLocation(FVector(0, 0, 40));
    FollowCamera->bUsePawnControlRotation = false;

    MoveComp = CreateDefaultSubobject<USMove>(TEXT("MoveComp"));
    BuffComp = CreateDefaultSubobject<USBuff>(TEXT("BuffComp"));
    InteractionItemComp = CreateDefaultSubobject<USInteractionItem>(TEXT("InteractionItemComp"));


    ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/RGY/Inputs/IMC_Survivor.IMC_Survivor'"));
    if (TempIMC.Succeeded()) {
        IMC_Survivor = TempIMC.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> TempIALook(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
    if (TempIALook.Succeeded()) {
        IA_Look = TempIALook.Object;
    }
}

// Called when the game starts or when spawned
void ASurvivor::BeginPlay()
{
	Super::BeginPlay();
	
    auto pc = Cast<APlayerController>(Controller);
    
    if (pc) {
        auto CameraManager = pc->PlayerCameraManager;
        CameraManager->ViewPitchMin = -15.0f;
        CameraManager->ViewPitchMax = 15.0f;
    }

    if(AnimInstance == nullptr && GetMesh()->GetAnimInstance()) {
        AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
    }

    auto gi = Cast<UIdentityNGameInstance>(GetWorld()->GetGameInstance());
    if(gi) {
        SurvivorData = gi->SurvivorDataMap.Find(Id);
        SetInitData();
    }
}

// Called every frame
void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 혹시 모르니 이중 확인
    if (bCrawl && HP == 0) {
        ProcessDeadGuage();
    }
}

// Called to bind functionality to input
void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        MoveComp->SetupInputBinding(input);
        InteractionItemComp->SetupInputBinding(input);

        // Looking
        input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASurvivor::Look);
    }
}

void ASurvivor::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(IMC_Survivor, 1);
        }
    }
}

float ASurvivor::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 1. 데미지 받을 수 있는 상황인지 체크
    bool bDamage = IsTakeDamage();
    if(!bDamage) return HP;

    // 2. 공포의 일격 여부 체크
    bool bAction = IsTakeAction();
    if (bAction) {
        // 공포의 일격 적용
        Damage = 2.0f;
    }

    // 3. 데미지 적용
    HP -= Damage;
    if(HP < 0) HP = 0.0f;

    // 3. HP 가 0 일 경우 쓰러짐
    if (HP == 0) {
        // 날아가서 쓰러지는 애니메이션
        // 해당 애니메이션이 종료될 경우 falling 이 false 가 되고 그러면 crawl 상태로 움직일 수 있게 됨
        AnimInstance->falling = true;
        FVector dir = GetActorLocation() - DamageCauser->GetActorLocation();
        LaunchCharacter(dir * 1000, false, false);

        // 과다출혈 타이머 초기화
        CrawlCurrentTime = 0.0f;

        bCrawl = true;
        //AnimInstance->bCrawl = true;

        if (MoveComp->bCrouch) {
            MoveComp->bCrouch = false;
            AnimInstance->bCrouch = false;
        }
    } else {
        // 대미지 애니메이션
        // 대미지 애니메이션 종료 후 Idle 상태로 변경되어 움직일 수 있게 됨
        State = ESurvivorState::DAMAGED;
        AnimInstance->State = ESurvivorState::DAMAGED;

        if (bCrawl) {
            bCrawl = false;
            //AnimInstance->bCrawl = false;
        }
        else if (MoveComp->bCrouch) {
            // 피격 시 웅크림 상태 해제
            MoveComp->ChangeCrouch(false);
        }
    }

    // 4. 피격 이동 가속 (쓰러짐 상태에서도 피격 이동 가속은 적용되어 있다 : 아드각) + 피격시 버프 적용
    BuffComp->DamagedBuff();

    CLog::Print(HP);
  
    return HP;
}

bool ASurvivor::IsOutofGame()
{
    if(State == ESurvivorState::READY || State == ESurvivorState::SUCCESS || State == ESurvivorState::FAIL) return true;

    return false;
}

void ASurvivor::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

bool ASurvivor::IsTakeDamage()
{
    // 무적 시간, 쓰러짐 상태, DAMAGED, BALLOONED, SIT
    if (IsOutofGame()) return false;
    if (bInvindibility || bCrawl) return false;
    if(State == ESurvivorState::DAMAGED || State == ESurvivorState::BALLOONED || State == ESurvivorState::SEAT) return false;

    return true;
}

bool ASurvivor::IsTakeAction()
{
    // 행동 중인가 여부
    if (IsOutofGame()) return false;
    if (State == ESurvivorState::IDLE|| State == ESurvivorState::DAMAGED
        || State == ESurvivorState::BALLOONED || State == ESurvivorState::SEAT) return false;
    //  || State == ESurvivorState::MOVE

    return true;
}

void ASurvivor::SetInitData()
{
    if (SurvivorData == nullptr) return;

    Name = SurvivorData->Name;

    MoveComp->SetInitData();
    BuffComp->SetInitData();
    InteractionItemComp->SetInitData();
}

void ASurvivor::ProcessDeadGuage()
{
    if(IsOutofGame()) return;

    // 치료 받는 중이면 일단 이 함수는 return 할 것


    CrawlCurrentTime += GetWorld()->GetDeltaSeconds();

    DeadGauge = CrawlCurrentTime / CrawlDeadTime;

    // 사망 : 탈락 UI 표시 필요
    if (CrawlCurrentTime >= CrawlDeadTime) {
        State = ESurvivorState::FAIL;
        CLog::Print("Die");

        // 상태가 Success 가 됐을 때도 bCrawl 상태가 풀린다.
        bCrawl = false;
        CrawlCurrentTime = 0.0f;

        auto gm = Cast<AIdentityNGameMode>(GetWorld()->GetAuthGameMode());
        gm->Eliminate(this);
    }
}

