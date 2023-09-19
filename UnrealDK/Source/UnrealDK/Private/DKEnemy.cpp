// Fill out your copyright notice in the Description page of Project Settings.


#include "DKEnemy.h"

// Unreal
#include "Math/UnrealMathUtility.h"
#include "PaperFlipbookComponent.h"

ADKEnemy::ADKEnemy()
{
    
}

// Save init values, use for respawn process
void ADKEnemy::BeginPlay()
{
    vSpawnLocation = GetActorLocation();
    rSpawnRotation = GetActorRotation();
}

// Called every frame
void ADKEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bWasDead)
    {
        if (bWasPatrolBehavior) 
        {
            // patrol bahavior
            PatrolTickSpawn();
        } else {
            // no patrol for this enemy, follow character
            FollowCharacter();
        }
    }
}

// Use to re-spawn
void ADKEnemy::PatrolTickSpawn()
{
    
}

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
            
            FVector2D DiffVector = FVector2D(PlayerLocation.X - EnemyLocation.X, PlayerLocation.Y - EnemyLocation.Y);
            FVector2D NormalVector = DiffVector.GetSafeNormal();
            
            // Geting angle
            float AngleInRadians = FMath::Atan2(NormalVector.Y, NormalVector.X);
            float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

            // Rotate if enemy look another location
            float RotateAngle = (AngleInDegrees < 90) ? -180.0f : 0.0f;
            FRotator NewRotation = FRotator(0.0f, 0.0f, RotateAngle);
            //GetSprite()->SetWorldRotation(NewRotation, ETeleportType::None);

            // Call event
            CharacterOnScreen(AngleInDegrees);
        }
    }
}

void ADKEnemy::CharacterOnScreen_Implementation(float Angle)
{
    UE_LOG(LogTemp, Warning, TEXT("Character on screen DKEnemy implementation...: %d"), Angle);
}