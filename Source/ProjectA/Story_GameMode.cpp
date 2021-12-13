// Fill out your copyright notice in the Description page of Project Settings.


#include "Story_GameMode.h"


float AStory_GameMode::ChanceForMonsterEncounter()
{
	float randomNumber = FMath::RandRange(0, 1000);

	int encounterValue = FMath::RoundToInt(1000 * monsterSummonChance) / 100;

	if (randomNumber <= encounterValue)
	{
		MonsterAttack();
	}
	return randomNumber;
}

void AStory_GameMode::MonsterAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "Monster wuerde jetzt kommen");
}