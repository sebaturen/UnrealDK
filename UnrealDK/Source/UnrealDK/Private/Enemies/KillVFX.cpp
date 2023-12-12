
#include "Enemies/KillVFX.h"
#include "PaperFlipbook.h"
#include "Kismet/KismetMathLibrary.h"

// Unreal
AKillVFX::AKillVFX()
{
	// Set Flipbook
	UPaperFlipbook* DefaultKillFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/Assets/Sprites/ObjectsAndEffects/kill_vfx"));
	ensureMsgf(DefaultKillFlipbook, TEXT("KillFlipbook not found!"));

	KillVFXFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(FName("Kill Flipbook"));
	KillVFXFlipbook->SetFlipbook(DefaultKillFlipbook);
	KillVFXFlipbook->SetupAttachment(RootComponent);

	// Force tick event
	PrimaryActorTick.bCanEverTick = true;
}

void AKillVFX::BeginPlay()
{
	Super::BeginPlay();

	// Translate effect and delete
	StartLocation = GetActorLocation();
	TargetLocation = FVector(StartLocation.X + MoveTranslation, StartLocation.Y, StartLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("Start... %d, %d, %d"), StartLocation.X, StartLocation.Y, StartLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("TargetLocation... %d, %d, %d"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z);

	KillVFXFlipbook->SetLooping(false);
}

// Called every frame
void AKillVFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TranslationEvent(DeltaTime);
	TEXT("Tick frame...");
}

void AKillVFX::TranslationEvent(float DeltaTime)
{
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, MoveSpeed * DeltaTime);
	SetActorLocation(NewLocation);
	//UE_LOG(LogTemp, Warning, TEXT("KillFX Moving... %d, %d"), NewLocation.X, TargetLocation.X);

	// Destroy if arrived
	if (NewLocation.X < TargetLocation.X)
	{
		//Destroy();
	}
}