// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DKItems.h"

// Sets default values
ADKItems::ADKItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPlaySound = true;

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