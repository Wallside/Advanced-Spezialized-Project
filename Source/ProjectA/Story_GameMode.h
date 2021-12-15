// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Story_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API AStory_GameMode : public AGameModeBase
{
 	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Monster/Combat")
	float monsterSummonChance{0.5f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	bool isMonsterActive{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	bool isMonsterOnCooldown{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	int monsterKillCountdown{15};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	int monsterCooldown{20};

	UFUNCTION(BlueprintCallable, Category = "Monster/Comabt")
	float ChanceForMonsterEncounter();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void MonsterAttack();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void MonsterCooldownTimerTick();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = "Monster/Combat")
	void OnMonsterCooldownTimerTick();

private:

	


public:

	

	
};
