// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayableCharacter.h"
#include "Story_GameMode.h"
#include "Interactable.generated.h"

UCLASS()
class PROJECTA_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();

	UPROPERTY(EditAnywhere, Category = "Interactable")
	bool collectable{false};

	UPROPERTY(EditAnywhere, Category = "Interactable")
	bool incomplete{false};

	UPROPERTY(EditAnywhere, Category = "Interactable")
	bool locked{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	bool isInteractable{false};

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Interactable|Item")
	FString objectName {"Name"};

	UPROPERTY(EditAnywhere, Category = "Interactable|Item")
	UTexture2D* objectIcon;

	UPROPERTY(EditAnywhere, Category = "Interactable|ObjectInformation")
	FString itemNeededToComplete;

	UPROPERTY(EditAnywhere, Category = "Interactable|ObjectInformation")
	UStaticMesh* completedMesh;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void OnCollected();

protected:

	AStory_GameMode* storymode;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Collect();

	void CompleteObject(APlayableCharacter* playerCharacter);

	void UnlockObject(APlayableCharacter* playerCharacter);

	void Interact(APlayableCharacter* playerCharacter);

};
