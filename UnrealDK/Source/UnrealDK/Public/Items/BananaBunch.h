// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DKItems.h"
#include "BananaBunch.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDK_API ABananaBunch : public ADKItems
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADKItems> BananaToSpawn;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* fCurveSpawn;

	UFUNCTION(BluePrintCallable, Category = "GameItems")
	void PickUpAnimation();
};
