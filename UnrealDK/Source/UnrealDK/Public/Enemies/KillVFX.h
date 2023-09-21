#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"

#include "KillVFX.generated.h"

UCLASS()
class UNREALDK_API AKillVFX : public AActor
{
	GENERATED_BODY()

public:

	AKillVFX();

	UPaperFlipbookComponent* KillVFXFlipbook;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Variables
	FVector StartLocation;
	FVector TargetLocation;
	float MoveSpeed = 10.0f;
	float MoveTranslation = 30.0f;
	
	// Functions
	void TranslationEvent(float DeltaTime);
};