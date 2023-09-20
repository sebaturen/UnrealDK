// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BananaBunch.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Items/Banana.h"

void ABananaBunch::PickUpAnimation()
{
	UWorld* const World = GetWorld();
	int timer = 0;
	int spawnedBananas = totalAnimBananas;

	if (World)
	{
		while (spawnedBananas >= 0)
		{
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, this, &ABananaBunch::AnimationBananaSpawn, timer / AnimationVelocity, false);
			timer++;
			spawnedBananas--;
		}
	}
}

void ABananaBunch::AnimationBananaSpawn()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		ADKItems* SpawnBanana = World->SpawnActor<ADKItems>(BananaToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnBanana)
		{
			SpawnBanana->AutoPlaySound = false;
			SpawnBanana->PickUp();
		}
		totalAnimBananas--;
		if (totalAnimBananas == 0)
		{
			Destroy();
		}
	}
}