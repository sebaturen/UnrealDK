// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DKItems.generated.h"

UCLASS(Abstract)
class UNREALDK_API ADKItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADKItems();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameItems")
	void PickUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameItems")
	bool AutoPlaySound;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};