// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "PaperFlipbook.h"

#include "DKEnemy.generated.h"

UENUM(BlueprintType)
enum class EDKDamageBehavior : uint8
{
    Dead = 0,
    Bounce = 1
};

/**
 * 
 */
UCLASS(Abstract)
class UNREALDK_API ADKEnemy : public APaperCharacter
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    ADKEnemy();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    ATargetPoint* StartMoveTargetPoint = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    ATargetPoint* EndMoveTargetPoint = nullptr;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy")
    void CharacterOnScreen(float Angle);
    virtual void CharacterOnScreen_Implementation(float Angle);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    void Patrol();
    void MovePatrol();

    // Configurations properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    bool bWasPatrolBehavior = true;
    UPROPERTY(EditAnywhere, Category = "Enemy")
    UPaperFlipbook* KillFlipbook;
    UPROPERTY(EditAnywhere, Category = "Enemy")
    USoundBase* HitSound;
    UPROPERTY(EditAnywhere)
    EDKDamageBehavior eDamageBehavior = EDKDamageBehavior::Dead;

    // Colision 
    UPROPERTY(EditAnywhere, Category = "Enemy")
    UBoxComponent* ScreenZone;
    UPROPERTY(EditAnywhere, Category = "Enemy")
    UBoxComponent* KillCollision;
    UPROPERTY(EditAnywhere, Category = "Enemy")
    UBoxComponent* DamageCollision;

    // On screen overlap event
    UFUNCTION()
    void OnScreenZoneOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnScreenZoneOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    UFUNCTION()
    void OnKillZoneOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnDamageZoneOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
   
    // TODO? how I can implement this delegate? On movement completed
    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void OnTickValidations();

private:
    
    // Variables
    FVector vSpawnLocation;
    FRotator rSpawnRotation;
    bool bWasDead = false;
    bool bWasRespawning = false;
    bool bIsPatrolOn = false;
    bool bIsDead = false;
    
    // Functions
    void PatrolTickSpawn();
    void FollowCharacter();
   
};
