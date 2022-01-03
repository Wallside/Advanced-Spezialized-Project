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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Monster/Combat")
	bool isProtected{false};

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
	void SetPlayerCamera(UCameraComponent* camera);

	UFUNCTION(BlueprintCallable, Category = "Monster/Combat")
	void ApplyPostProcessSettingChanges(); 
	
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ChangeCrosshair();

protected:

	AStory_GameMode* storymode;

	UCameraComponent* playerCameraComponent;

	PostProcessChangeValues changeValues;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FHitResult ObjectToInteract();	

	void StopTerrorRadius();
	
	void CalculatePostProcessSettingDifference();

	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TriggerTerrorRadius();

	Inventory* inventory = new Inventory();
};
