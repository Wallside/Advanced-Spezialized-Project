// Fill out your copyright notice in the Description page of Project Settings.


#include "Story_GameMode.h"
#include "PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"



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
	Cast<APlayableCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->TriggerTerrorRadius();
}

void AStory_GameMode::MonsterCooldownTimerTick()
{
	OnMonsterCooldownTimerTick();
}

void AStory_GameMode::TriggerGameOver() 
{
	GameOver();
}

void AStory_GameMode::ChangeLevel(FName newLevel)
{
	UGameplayStatics::OpenLevel(GetWorld(), newLevel);
}

void AStory_GameMode::TriggerSaveGame()
{
	SaveGame();
}

void AStory_GameMode::TriggerLoadGame()
{
	LoadGame();
}

void AStory_GameMode::TriggerAdjustVolume()
{
	AdjustVolume();
}

