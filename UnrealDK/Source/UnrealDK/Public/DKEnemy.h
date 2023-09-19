// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DKEnemy.generated.h"

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
    
    // Behavior definition
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    bool bWasPatrolBehavior = false;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    //UPaperFlipbookComponent* 

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy")
	void CharacterOnScreen(float Angle);
    virtual void CharacterOnScreen_Implementation(float Angle);
    
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    void Patrol();
    
private:
    
    // Variables
    FVector vSpawnLocation;
    FRotator rSpawnRotation;
    bool bWasDead = false;
    
    // Functions
    void PatrolTickSpawn();
    void FollowCharacter();
    
};
