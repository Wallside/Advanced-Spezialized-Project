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

	UPROPERTY(EditAnywhere, Category = "MonsterAI")
	float monsterSummonChance{0.5f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isMonsterActive{false};

	UFUNCTION(BlueprintCallable, Category = "MonsterAI")
	float ChanceForMonsterEncounter();

	UFUNCTION(BlueprintCallable, Category = "MonsterAI")
	void MonsterAttack();

private:

	
};
