// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "Interactable.h"
// Sets default values
APlayableCharacter::APlayableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayableCharacter::Interact() 
{
	FHitResult hit = ObjectToInteract();

	AInteractable* hitObject = Cast<AInteractable>(hit.Actor);

	if (hitObject)
	{
		if (hitObject->collectable)
		{
			Item* newItem = new Item(hitObject->objectName, hitObject->objectIcon);
			inventory->AddItemToInventory(newItem);
			hitObject->Collect();
		}
		else if (hitObject->incomplete)
		{			
			hitObject->CompleteObject(this);
		}
		
	}	
}

FString APlayableCharacter::InventoryContent()
{
	
	FString content;
	for (int i = 0; i < 5; i++)
	{
		if (inventory->inventoryList[i] != NULL)
		{
			content += (inventory->inventoryList[i]->name + ", ");
		}
		else
		{
			content += "Empty, ";
		}
	}
	return content;	
}

FHitResult APlayableCharacter::ObjectToInteract()
{
	FVector playerLocation;
	FRotator playerRotation;
	FVector endTrace = FVector::ZeroVector;

	APlayerController* const playerCharacter = GetWorld()->GetFirstPlayerController();

	if (playerCharacter)
	{
		playerCharacter->GetPlayerViewPoint(playerLocation, playerRotation);
		endTrace = playerLocation + (playerRotation.Vector() * interactRange);		
	}

	FCollisionQueryParams traceParams(SCENE_QUERY_STAT(ObjectToInteract), false, GetInstigator());
	FHitResult hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(hit, playerLocation, endTrace, ECC_Visibility, traceParams);

	return hit;
}

FString APlayableCharacter::GetItemNameInIndex(int index)
{
	if (inventory->inventoryList[index] != NULL)
	{
		return inventory->inventoryList[index]->name;
	}
	else
	{
		return "";
	}
	
}

UTexture2D* APlayableCharacter::GetItemIconInIndex(int index)
{
	if (inventory->inventoryList[index] != NULL)
	{
		return inventory->inventoryList[index]->image;
	}
	else
	{
		return NULL;
	}
}

void APlayableCharacter::SwitchProtectionState() 
{
	if (isProtected)
	{
		isProtected = false;
	}
	else
	{
		isProtected = true;
	}
}

void APlayableCharacter::Defend()
{

}

void APlayableCharacter::CheckForStorymode() 
{
	//if (storymode != NULL)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Storymode gefunden");
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Storymode nicht gefunden");
	//}
}

