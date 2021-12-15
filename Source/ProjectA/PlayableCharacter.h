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
	FPostProcessSettings normalSettings;

	UPROPERTY(EditAnywhere, Category = "Monster/Combat")
	FPostProcessSettings terrorSettings;

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

	UFUNCTION(BlueprintCallable, Category = "Monster/Attack")
	void SetPlayerCamera(UCameraComponent* camera);

protected:

	AStory_GameMode* storymode;

	UCameraComponent* playerCameraComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FHitResult ObjectToInteract();	

	void StopTerrorRadius();
	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TriggerTerrorRadius();

	Inventory* inventory = new Inventory();
};
