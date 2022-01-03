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
				TArray<UActorComponent*> components;
				this->GetComponents(components);
				for (int j = 0; j < components.Num(); j++)
				{
					UStaticMeshComponent* incompleteMesh = Cast<UStaticMeshComponent>(components[j]);
					if (incompleteMesh)
					{
	
						incompleteMesh->SetStaticMesh(completedMesh);
						this->incomplete = false;
						this->isInteractable = true;
						playerCharacter->inventory->RemoveItemFromInventory(i);
					}
				}
				break;
			}
		}
		else if (i == 4)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "Benoetigtes Item nicht in Slot Inventar");
		}
	}	
}

void AInteractable::UnlockObject(APlayableCharacter* playerCharacter)
{

}

void AInteractable::Interact(APlayableCharacter* playerCharacter)
{
	if (storymode->isMonsterActive)
	{
		playerCharacter->Defend();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Ich kann hiermit Momentan nicht interagieren");
	}
}

