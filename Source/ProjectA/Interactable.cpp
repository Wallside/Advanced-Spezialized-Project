// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	storymode = Cast<AStory_GameMode>(GetWorld()->GetAuthGameMode());
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::Collect()
{
	OnCollected();
}

void AInteractable::CompleteObject(APlayableCharacter* playerCharacter) 
{
	for (int i = 0; i < 5; i++)
	{
		if (playerCharacter->inventory->inventoryList[i] != NULL)
		{
		
			if (playerCharacter->inventory->inventoryList[i]->name == itemNeededToComplete)
			{
				incomplete = false;
				playerCharacter->inventory->RemoveItemFromInventory(i);				
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, "Item Completed");
				OnObjectCompleted();
				break;
			}
		}
		else if (i == 4)
		{
			playerCharacter->OnIncompleteObjectInteract(this);
			firstInteraction = false;
		}
	}	
}

void AInteractable::UnlockObject(APlayableCharacter* playerCharacter)
{
	for (int i = 0; i < 5; i++)
	{
		if (playerCharacter->inventory->inventoryList[i] != NULL)
		{
			if (playerCharacter->inventory->inventoryList[i]->name == itemNeededToComplete)
			{
				locked = false;
				playerCharacter->inventory->RemoveItemFromInventory(i);
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, "Door Unlocked");
				OnObjectUnlocked();
				break;
			}
		}
		else if (i == 4)
		{
			playerCharacter->OnIncompleteObjectInteract(this);
			firstInteraction = false;
		}
	}
}

void AInteractable::Interact(APlayableCharacter* playerCharacter, UStaticMeshComponent* component)
{
	hitComponentName = component->GetName();
	if (objectType == Collectable)
	{
		Item* newItem = new Item(objectName, objectIcon);
		playerCharacter->inventory->AddItemToInventory(newItem);
		playerCharacter->CollectSound();
		Collect();
	}
	else if (objectType == Defendable)
	{
		if (playerCharacter->GetStoryMode()->isMonsterActive)
		{
			playerCharacter->Defend();
			OnInteract();
		}
	}
	else if (objectType == CanBeOpened)
	{
		OpenAndClose();
		OnInteract();
	}	
	else if (objectType == Inspectable)
	{
		playerCharacter->SafeInspectableObject(objectIcon);
		playerCharacter->OnInspect();
	}
}

