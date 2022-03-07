// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "Item.h"
#include "Story_GameMode.h"
#include "Camera/CameraComponent.h"
#include "Engine/Scene.h"
#include "PlayableCharacter.generated.h"

struct PostProcessChangeValues 
{
	float chromaticAberrationIntensitiy;
	float imageEffectsVignetteIntensity, imageEffectsGrainJitter, imageEffectsGrainIntensity;
	float colorGradingGlobalSaturationR, colorGradingGlobalSaturationG, colorGradingGlobalSaturationB, colorGradingGlobalSaturationY;
	float colorGradingGlobalOffsetR, colorGradingGlobalOffsetG, colorGradingGlobalOffsetB, colorGradingGlobalOffsetY;

	//Fmod Variable
	float monsterIntensity;
};

UCLASS()
class PROJECTA_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayableCharacter();

	UPROPERTY(EditAnywhere, Category = "Interact")
	float interactRange{10};

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Monster/Combat")
	bool isRecovering{false};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Monster/Combat")
	int recoveryTime{5};

	UPROPERTY(BlueprintReadOnly, Category = "Audio/FMOD")
	FString roomName;

	UPROPERTY(EditAnywhere, Category = "Monster/Combat")
	FPostProcessSettings normalSetting;

	UPROPERTY(EditAnywhere, Category = "Monster/Combat")
	FPostProcessSettings terrorSetting;

	UPROPERTY(EditAnywhere, Category = "Monster/Combat")
	FPostProcessSettings activeSetting;

	UPROPERTY(EditAnywhere, Category = "User Interface")
	UTexture2D* normalCrosshair;

	UPROPERTY(EditAnywhere, Category = "User Interface")
	UTexture2D* interactiveCrosshair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "User Interface")
	UTexture2D* activeCrosshair;

	UPROPERTY(BlueprintReadOnly, Category = "User Interface")
	UTexture2D* inspectedObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster/Combat")
	float monsterIntensity = 0;

	UPROPERTY(BlueprintReadOnly, Category = "FMOD/Variables")
	FString tempo; 

	UPROPERTY(BlueprintReadOnly, Category = "FMOD/Variables")
	int raeumlichkeiten;

	UPROPERTY(BlueprintReadOnly, Category = "FMOD/Variables")
	FString boden;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FString InventoryContent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FString GetItemNameInIndex(int index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UTexture2D* GetItemIconInIndex(int index);

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void Defend();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void InputDefend();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void SetPlayerCamera(UCameraComponent* camera);

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void ApplyPostProcessSettingChanges(int index); 
	
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ChangeCrosshair();

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void RecoveryTimerTick();

	UFUNCTION(BlueprintImplementableEvent, Category = "Monster/Combat")
	void OnRecoveryTimerTick();

	UFUNCTION(BlueprintCallable, Category = "Monster/Comabt")
	void NormalizePostProcessingSettings();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void PlayCollectSound();

	UFUNCTION(BlueprintCallable, Category = "Audio/FMOD")
	void QueueRoomSound(FString enteredRoom);

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void RefreshRoomSound();

	UFUNCTION(BlueprintImplementableEvent, Category = "User Interface")
	void OnInspect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void PlayMonsterAttackAudio();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void OnObjectFirstInteraction(AInteractable* object);

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void TriggerTerrorRadius();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void ToggleMonsterSound();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void GetMonsterIntensity();

	UFUNCTION(BlueprintCallable)
	void ViewChanges();

	UFUNCTION(BlueprintCallable, Category = "Audio/FMOD")
	void TriggerAudioEvent(AInteractable* object);

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void PlayAudioEvent(AInteractable* object);

	

protected:	
	
	AStory_GameMode* storymode;

	UCameraComponent* playerCameraComponent;

	PostProcessChangeValues changeValues;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FHitResult ObjectToInteract();	

	void StopTerrorRadius();
	
	void CalculatePostProcessSettingDifference(int index);
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	void CollectSound();

	void SafeInspectableObject(UTexture2D* inspectableObject);

	AStory_GameMode* GetStoryMode();

	Inventory* inventory = new Inventory();
};
