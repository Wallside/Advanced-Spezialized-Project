// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "Item.h"
#include "Interactable.h"
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

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Collect();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FString InventoryContent();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FHitResult ObjectToInteract();	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	Inventory* inventory = new Inventory();


};
