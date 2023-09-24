// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DKItems.h"

// Unreal
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADKItems::ADKItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPlaySound = true;

	// SetDefault Component
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
	Sprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADKItems::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADKItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADKItems::PickUp_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup implemented on base..."));
}

void ADKItems::MoveToCorner()
{
	MoveToCornerStartPosition = GetActorLocation();
	MoveToCornerStartPosition.Y = 100.0f;
	SetActorLocation(MoveToCornerStartPosition);

	MoveToCornerStartTime = GetWorld()->GetTimeSeconds();
	MoveToCornerFinalTime = MoveToCornerStartTime + MoveToCornerDuration;

	GetWorld()->GetTimerManager().SetTimer(MoveToCornerInterpolacionHandle, this, &ADKItems::MoveToCornerInterpolation, 0.01f, true);
}

void ADKItems::MoveToCornerInterpolation()
{
	float CurrenTime = GetWorld()->GetTimeSeconds();

	// geting destination position 10,10 in screen to world
	APlayerController* Player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenPosition(10.0f, 10.0f);
	FVector TargetLocation;
	FVector WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(Player, ScreenPosition, TargetLocation, WorldDirection);

	if (CurrenTime >= MoveToCornerFinalTime)
	{
		SetActorLocation(TargetLocation);
		GetWorld()->GetTimerManager().ClearTimer(MoveToCornerInterpolacionHandle);
		Destroy();
	}
	else 
	{
		float Alpha = (CurrenTime - MoveToCornerStartTime) / MoveToCornerDuration;
		SetActorLocation(FMath::Lerp(MoveToCornerStartPosition, TargetLocation, Alpha));
	}
}