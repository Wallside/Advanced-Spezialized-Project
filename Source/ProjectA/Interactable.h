// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayableCharacter.h"
#include "Story_GameMode.h"
#include "Interactable.generated.h"


UENUM()
enum ObjectType
{
	Collectable, Defendable, CanBeOpened, Inspectable, None
};


UCLASS()
class PROJECTA_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|ObjectInformation")
	bool incomplete{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|ObjectInformation")
	bool locked{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	bool firstInteraction = true;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Interactable|ObjectInformation")
	FString objectName {"Name"};

	UPROPERTY(BlueprintReadOnly, Category = "Interactable|Elevator")
	int status;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable|Elevator")
	int position;

	UPROPERTY(BlueprintReadOnly, Category = "Audio/FMOD")
	int songChoice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|ObjectInformation")
	UTexture2D* objectIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Inspectable")
	int imageWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable|Inspectable")
	int imageHeight;

	UPROPERTY(EditAnywhere, Category = "Interactable|ObjectInformation")
	FString itemNeededToComplete;

	UPROPERTY(EditAnywhere, Category = "Interactable|ObjectInformation")
	TEnumAsByte<ObjectType> objectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Envelopment")
	int objectID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	FString hitComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AInteractable*> refereance;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void OnCollected();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void OnObjectCompleted();
		
	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void OnObjectUnlocked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void OnInteract();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void OpenAndClose(UStaticMeshComponent* component);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Envelopment")
    void ApplyWindForceChanges(float newWindForce);

	UFUNCTION(BlueprintCallable, Category = "Audio/FMOD")
	void TriggerAudioEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio/FMOD")
	void AudioEvent();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void Collect();

	/*
		0 = Drawer, 1 = Writing Desk, 2 = RezeptionX, 3 = RezeptionY
	*/
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void TriggerMoveObject(int index);

	/*
		0 = Drawer, 1 = Writing Desk, 2 = RezeptionX, 3 = RezeptionsY
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void MoveObject(int index);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void TriggerClearAllReferences(AInteractable* itemToBeCleared);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void ClearAllReferences(AInteractable* itemToBeCleared);

protected:

	AStory_GameMode* storymode;

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
public:	

	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CompleteObject(APlayableCharacter* playerCharacter);

	void UnlockObject(APlayableCharacter* playerCharacter);

	void Interact(APlayableCharacter* playerCharacter, UStaticMeshComponent* component);

	

};
