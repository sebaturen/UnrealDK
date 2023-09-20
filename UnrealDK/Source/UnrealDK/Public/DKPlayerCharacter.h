#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "DKPlayerCharacter.generated.h"

/**
* 
*/
UCLASS(Abstract)
class UNREALDK_API ADKPlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	ADKPlayerCharacter();

	UFUNCTION()
	void ActorOnOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void OnDamage();
};