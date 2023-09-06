// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BananaBunch.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Items/Banana.h"

void ABananaBunch::PickUpAnimation()
{
	UWorld* const World = GetWorld();
	int totalBanana = 10;

	if (World)
	{
		while (totalBanana > 0)
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Y = 100;

			ADKItems* SpawnBanana = World->SpawnActor<ADKItems>(BananaToSpawn, SpawnLocation, FRotator::ZeroRotator);

			if (SpawnBanana)
			{
				UE_LOG(LogTemp, Warning, TEXT("Ok spoanwed: %s"), *SpawnLocation.ToString());
			}

			totalBanana--;
		}
	}
}