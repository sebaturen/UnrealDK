// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperCharacter.h"
#include "DKItems.generated.h"

UENUM(BlueprintType)
enum class EDKMoveBehavior : uint8
{
	Left,
	Right,
	Midle
};

UCLASS(Abstract)
class UNREALDK_API ADKItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADKItems();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DK Items")
	void PickUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DK Items")
	bool AutoPlaySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DK Items")
	UPaperFlipbookComponent* Sprite = nullptr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "DK Items")
	void MoveToCorner(EDKMoveBehavior MoveTo = EDKMoveBehavior::Left);
	void MoveToCornerInterpolation();

private:

	FVector MoveToCornerStartPosition;
	float MoveToCornerStartTime;
	float MoveToCornerFinalTime;
	FVector2D MoveToCornerScreenPosition;
	FTimerHandle MoveToCornerInterpolacionHandle;
	const float MoveToCornerDuration = 0.5f;

};
