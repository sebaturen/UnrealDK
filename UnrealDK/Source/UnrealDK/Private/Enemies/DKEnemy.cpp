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

    // Empty Flipbook
    EmptyFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/Assets/Sprites/EmptyFlipbook"));
    ensureMsgf(DefaultHitCue, TEXT("Empty EmptyFlipbook not found!"));
    //EmptyFlipbook->keyframe
    /*UPaperSprite* EmptySprite = NewObject<UPaperSprite>();
    EmptyFlipbook->
    EmptyFlipbook->AddNewFrame(EmptySprite);
    EmptyFlipbook->SetFramesPerSecond(1.0f);*/

}

// Save init values, use for respawn process
void ADKEnemy::BeginPlay()
{
    Super::BeginPlay();
    vSpawnLocation = GetActorLocation();
    rSpawnRotation = GetActorRotation();

    // Base Flipbook
    SourceFlipbook = GetSprite()->GetFlipbook();
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
        //UE_LOG(LogTemp, Warning, TEXT("TickValiation | %s"), *GetName(), WasRecentlyRendered());
        // need respawn, and is in spawn location and not rendering
        if (bWasRespawning && GetActorLocation().Equals(vSpawnLocation) && !WasRecentlyRendered())
        {
            if (iRespawningGracePeriod > 0)
            {
                iRespawningGracePeriod--;
            }
            else
            {
                // Check if player is on screen...
                bWasRespawning = false;
                GetSprite()->SetFlipbook(SourceFlipbook);
                iRespawningGracePeriod = 3;
                UE_LOG(LogTemp, Warning, TEXT("TickValiation | %s | %d | %s"), *GetName(), WasRecentlyRendered(), *GetActorLocation().ToString());
            }
        }
        // have petrol behavior, and is patrol on and isn't rendering, STOP imedialty
        else if (bWasPatrolBehavior && bWasPatrolOn && !WasRecentlyRendered())
        {
            GetCharacterMovement()->StopActiveMovement();

            // Restore sprite
            GetSprite()->SetFlipbook(EmptyFlipbook);
            GetSprite()->SetLooping(true);

            // Restore flag to originals
            bWasPatrolOn = false;
            bWasRespawning = true;
            bMovingToFirst = true;
            bMoveChangeDirection = false;
            bIsFlipStart = false;

            // Teleport to new location
            TeleportTo(vSpawnLocation, rSpawnRotation);
        }
        // have petrol behavior and is NOT start patrol and is NOT respawning and is rendering, DO PATROL
        else if (bWasPatrolBehavior && !bWasPatrolOn && !bWasRespawning && WasRecentlyRendered())
        {
            Patrol();
        }
        // dons't have petrol behavior and is rendering, FOLLO CHARACTER (looking)
        else if (!bWasPatrolBehavior && WasRecentlyRendered())
        {
            FollowCharacter();
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
    
    if (PlayerController && !bIsFlipStart)
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
            /*FRotator ActualRotation = GetSprite()->GetRelativeRotation();
            UE_LOG(LogTemp, Warning, TEXT("Actual Rotate %d | Angle: %d"), ActualRotation.Yaw, AngleInDegrees);
            if (AngleInDegrees < 90 && AngleInDegrees > -90 && ActualRotation.Yaw == 180.0f)
            {
                MoveFlip();
            }*/
            FRotator NewRotation = GetSprite()->GetRelativeRotation();
            float oldRotation = NewRotation.Yaw;
            NewRotation.Yaw = (AngleInDegrees < 90 && AngleInDegrees > -90) ? 0.0f: 180.0f;
            if (oldRotation != NewRotation.Yaw)
            {
                MoveFlip();
                UE_LOG(LogTemp, Warning, TEXT("Actual Rotate  Change yaw"));
            }
            //GetSprite()->SetRelativeRotation(NewRotation); // , false, nullptr, ETeleportType::None);*/
            
            // Call event
            OnCharacterOnScreen(AngleInDegrees);
        }
    }
}

void ADKEnemy::OnCharacterOnScreen_Implementation(float Angle)
{
    //UE_LOG(LogTemp, Warning, TEXT("Character on screen implemented on base..."));
}

/**
* =============================
* Start - PATROL BEHAVIOR
* =============================
*/
void ADKEnemy::Patrol()
{
    bWasPatrolOn = true;
    MovePatrol();
}

void ADKEnemy::MovePatrol()
{
    AAIController* AIPawnController = Cast<AAIController>(GetController());
    if (!bIsDead && bWasPatrolOn)
    {
        if (bMoveChangeDirection)
        {
            MoveFlip();
        }
        else 
        {
            ATargetPoint* MoveDirection = bMovingToFirst ? StartMoveTargetPoint : EndMoveTargetPoint;
            bMovingToFirst = !bMovingToFirst;

            if (AIPawnController && MoveDirection)
            {
                AIPawnController->MoveToActor(MoveDirection, -1.0f, true, false);
                AIPawnController->ReceiveMoveCompleted.AddUniqueDynamic(this, &ADKEnemy::OnMoveCompleted);
            }
        }
    }
    else
    {
        if (AIPawnController)
        {
            AIPawnController->ReceiveMoveCompleted.RemoveDynamic(this, &ADKEnemy::OnMoveCompleted);
        }
    }
}

void ADKEnemy::MoveFlip()
{
    if (!bIsDead && !bWasRespawning)
    {
        if (!bIsFlipStart)
        {
            GetSprite()->SetFlipbook(FlipFlipbook);
            GetSprite()->SetLooping(false);
            GetSprite()->OnFinishedPlaying.AddUniqueDynamic(this, &ADKEnemy::MoveFlip);
            bIsFlipStart = true;
            return;
        }

        FRotator NewRotation = GetSprite()->GetRelativeRotation();
        NewRotation.Yaw = (NewRotation.Yaw == 0.0f) ? 180.0f : 0.0f;
        GetSprite()->SetRelativeRotation(NewRotation);

        if (bMoveChangeDirection)
        {
            bMoveChangeDirection = false;
            //GetSprite()->Reverse();
            GetSprite()->Play();
        }
        else
        {
            GetSprite()->SetFlipbook(SourceFlipbook);
            GetSprite()->SetLooping(true);
            GetSprite()->PlayFromStart();

            bIsFlipStart = false;
            if (bWasPatrolOn) MovePatrol();
        }
    }
}

void ADKEnemy::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (!bWasRespawning)
    {
        bMoveChangeDirection = true;
        MovePatrol();
    }
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
            //UE_LOG(LogTemp, Warning, TEXT("Start... New actor in %d, %d, %d"), Post.X, Post.Y, Post.Z);
            //UE_LOG(LogTemp, Warning, TEXT("Start... New actor in %s"), *CurrentEnemy->GetActorLabel());

            World->SpawnActor<AKillVFX>(KillVFX, GetActorLocation(), FRotator::ZeroRotator);
            // TODO: Replace if move left or right
        }

        // Dead flipbook
        GetSprite()->SetLooping(false);
        GetSprite()->SetFlipbook(DieFlipbook);

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