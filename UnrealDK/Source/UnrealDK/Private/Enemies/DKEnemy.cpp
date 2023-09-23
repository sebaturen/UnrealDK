// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/DKEnemy.h"
#include "DKPlayerCharacter.h"

// DK Game
#include "Enemies/KillVFX.h"

// Unreal
#include "Math/UnrealMathUtility.h"
#include "PaperFlipbookComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ADKEnemy::ADKEnemy()
{
    // Default values:

    // Hit sound
    USoundCue* DefaultHitCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Audio/SFX/Enemies/Enemy_Hit_DKC3_Cue"));
    ensureMsgf(DefaultHitCue, TEXT("Default Hit cue not found!"));
    HitSound = DefaultHitCue;

    // Set Screen zone collision
    ScreenZone = CreateDefaultSubobject<UBoxComponent>(FName("Screen Zone"));
    FVector ScreenZoneScale(9.5f, 5.75f, 5.75f);
    FVector ScreenZoneBoxExtent(32.0f, 32.0f, 32.0f);
    ScreenZone->SetupAttachment(RootComponent);
    ScreenZone->SetWorldScale3D(ScreenZoneScale);
    ScreenZone->SetBoxExtent(ScreenZoneBoxExtent);
    ScreenZone->OnComponentBeginOverlap.AddDynamic(this, &ADKEnemy::OnScreenZoneOverlapBegin);
    ScreenZone->OnComponentEndOverlap.AddDynamic(this, &ADKEnemy::OnScreenZoneOverlapEnd);
    ScreenZone->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    // Collision
    GetCharacterMovement()->SetPlaneConstraintEnabled(true);
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));

    // Set Kill Collision
    KillCollision = CreateDefaultSubobject<UBoxComponent>(FName("Kill Collision"));
    KillCollision->SetupAttachment(RootComponent);
    KillCollision->OnComponentBeginOverlap.AddDynamic(this, &ADKEnemy::OnKillZoneOverlapBegin);
    KillCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    // Set Damage Zone
    DamageCollision = CreateDefaultSubobject<UBoxComponent>(FName("Damage Collision"));
    DamageCollision->SetupAttachment(RootComponent);
    DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ADKEnemy::OnDamageZoneOverlapBegin);
    DamageCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    // Default movement speed
    GetCharacterMovement()->MaxWalkSpeed = 150.0f;
}

// Save init values, use for respawn process
void ADKEnemy::BeginPlay()
{
    Super::BeginPlay();
    vSpawnLocation = GetActorLocation();
    rSpawnRotation = GetActorRotation();
}

// Called every frame
void ADKEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    OnTickValidations();
}

void ADKEnemy::OnTickValidations()
{
    if (!bIsDead)
    {
        if (bWasPatrolBehavior)
        {
            PatrolTickSpawn();
        }
        else {
            FollowCharacter();
        }
    }
}

// Use to re-spawn
void ADKEnemy::PatrolTickSpawn()
{
    if (bWasRespawning) // if need respawn
    {
        // valid if character is out off screen
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            ACharacter* PlayerCharacrter = PlayerController->GetCharacter();
            UE_LOG(LogTemp, Warning, TEXT("PatrolTickSpawn ScreenZone %s - %s"), *KillCollision->GetName(), *GetName());
            if (!ScreenZone->IsOverlappingActor(PlayerCharacrter)) // User out screen collision box
            {
                GetSprite()->SetVisibility(true);
                bWasRespawning = false;
            }
        }
    }
}

/**
* Used to "look" the Player Character, used from DK Kremling or Bearth enemy
* 
* Calculate angle betwen Enemy and Player, and use 180-0 angle to determinate the sprite
* or rotation
*/
void ADKEnemy::FollowCharacter()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    
    if (PlayerController)
    {
        ACharacter* PlayerCharacter = PlayerController->GetCharacter(); // TODO: I can cast using a c++ class from character, now I use generic because the character only exist on blueprint
        if (PlayerCharacter)
        {
            FVector PlayerLocation = PlayerCharacter->GetActorLocation();
            FVector EnemyLocation = GetActorLocation();
            
            FVector2D DiffVector = FVector2D(PlayerLocation.X - EnemyLocation.X, PlayerLocation.Z - EnemyLocation.Z);
            FVector2D NormalVector = DiffVector.GetSafeNormal();
            
            // Geting angle
            float AngleInRadians = FMath::Atan2(NormalVector.Y, NormalVector.X);
            float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

            // Rotate if enemy look another location
            float RotateAngle = (AngleInDegrees < 90 && AngleInDegrees > -90) ? -180.0f : 0.0f;
            FRotator NewRotation = FRotator(0.0f, RotateAngle, 0.0f);
            GetSprite()->SetWorldRotation(NewRotation); // , false, nullptr, ETeleportType::None);
            
            // Call event
            OnCharacterOnScreen.Broadcast(AngleInDegrees);
        }
    }
}

/**
* =============================
* Start - PATROL BEHAVIOR
* =============================
*/
void ADKEnemy::Patrol()
{
    bIsPatrolOn = true;
    MovePatrol();
}

void ADKEnemy::MovePatrol()
{
    if (!bIsDead && bIsPatrolOn)
    {
        ATargetPoint* MoveDirection = bMovingToFirst ? StartMoveTargetPoint : EndMoveTargetPoint;
        bMovingToFirst = !bMovingToFirst;

        AAIController* AIPawnController = Cast<AAIController>(GetController());
        if (AIPawnController && MoveDirection)
        {
            AIPawnController->MoveToActor(MoveDirection, -1.0f, true, false);
            AIPawnController->ReceiveMoveCompleted.AddUniqueDynamic(this, &ADKEnemy::OnMoveCompleted);
        }
    }
}

void ADKEnemy::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    MovePatrol();
}

/**
* =============================
* End - PATROL BEHAVIOR
* =============================
*/

/**
* =============================
* Start - COLLISION BEHAVIOR (on kill collision, damange or bounce)
* =============================
*/

void ADKEnemy::OnScreenZoneOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<ADKPlayerCharacter>(OtherActor)) // if overlap is character
    {
        if (!bWasRespawning)
        {
            if (bWasPatrolBehavior && !bIsPatrolOn)
            {
                // Patrol active
                Patrol();
            }
            else {
                // Character on screen
                //CharacterOnScreen(0f);
            }
        }
    }
}

void ADKEnemy::OnScreenZoneOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Cast<ADKPlayerCharacter>(OtherActor) && !bIsDead) // if overlap is character
    {
        // Disable patrol and prepare for re-spawn
        bIsPatrolOn = false;
        bWasRespawning = true;
        GetCharacterMovement()->StopActiveMovement();
        GetSprite()->SetVisibility(false);
        TeleportTo(vSpawnLocation, rSpawnRotation);
    }
}

void ADKEnemy::OnKillZoneOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<ADKPlayerCharacter>(OtherActor) && !bIsDead && !bWasRespawning && eDamageBehavior == EDKDamageBehavior::Dead)
    {
        APawn* CurrentEnemy = Cast<AAIController>(GetController())->GetPawn();
        bIsDead = true;
        UGameplayStatics::PlaySound2D(this, HitSound);

        // Kill effect
        UWorld* const World = GetWorld();
        if (World)
        {
            TSubclassOf<AKillVFX> KillVFX = AKillVFX::StaticClass();
            FVector Post = CurrentEnemy->GetActorLocation();
            UE_LOG(LogTemp, Warning, TEXT("Start... New actor in %d, %d, %d"), Post.X, Post.Y, Post.Z);
            UE_LOG(LogTemp, Warning, TEXT("Start... New actor in %s"), *CurrentEnemy->GetActorLabel());

            World->SpawnActor<AKillVFX>(KillVFX, GetActorLocation(), FRotator::ZeroRotator);
            // TODO: Replace if move left or right
        }

        // Dead flipbook
        GetSprite()->SetLooping(false);
        GetSprite()->SetFlipbook(KillFlipbook);

        // Dead Movement
        GetCharacterMovement()->StopActiveMovement();
        FVector CurrentLocation = GetActorLocation();
        SetActorLocation(FVector(CurrentLocation.X, 100.0f, CurrentLocation.Z));
        // translation to new position
    }
}

void ADKEnemy::OnDamageZoneOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ADKPlayerCharacter* PlayerCharacter = Cast<ADKPlayerCharacter>(OtherActor);
    if (PlayerCharacter && !bIsDead && !bWasRespawning)
    {
        switch (eDamageBehavior)
        {
        case EDKDamageBehavior::Dead:
            PlayerCharacter->OnDamage();
            break;
        case EDKDamageBehavior::Bounce:
            break;
        default:
            break;
        }
    }
}

/**
* =============================
* End - COLLISION BEHAVIOR (on kill collision, damange or bounce)
* =============================
*/