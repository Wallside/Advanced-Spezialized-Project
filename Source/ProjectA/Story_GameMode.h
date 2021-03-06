// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "Interactable.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	bool isMonsterDisabled{ false };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	bool isMonsterActive{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	bool isMonsterOnCooldown{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	bool isGamePaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	int monsterKillCountdown{15};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster/Combat")
	int monsterCooldown{20};
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Envelopment")
    float windForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "System")
	float masterVolume{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "System")
	float musicVolume{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "System")
	float voiceVolume{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "System")
	float sfxVolume{1};

	UFUNCTION(BlueprintCallable, Category = "Monster/Comabt")
	float ChanceForMonsterEncounter();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void MonsterAttack();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void MonsterCooldownTimerTick();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void TriggerGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = "Monster/Combat")
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ChangeLevel(FName newLevel);

	UFUNCTION(BlueprintImplementableEvent, Category = "Monster/Combat")
	void OnMonsterCooldownTimerTick();	

	UFUNCTION(BlueprintImplementableEvent, Category = "System")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "System")
	void TriggerSaveGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "System")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "System")
	void TriggerLoadGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "System")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "System")
	void TriggerAdjustVolume();

	UFUNCTION(BlueprintImplementableEvent, Category = "System")
	void AdjustVolume();



private:

	


public:

	

	
};
