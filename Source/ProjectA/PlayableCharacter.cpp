// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"

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

void APlayableCharacter::Collect() 
{
	FHitResult hit = ObjectToInteract();

	AInteractable* hitObject = Cast<AInteractable>(hit.Actor);

	if (hitObject && hitObject->collectable)
	{
		Item* newItem = new Item(hitObject->objectName, hitObject->objectIcon);
		inventory->AddItemToInventory(newItem);
		hitObject->Collect();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, inventory->inventoryList[2]->name);
		if (inventory->inventoryList[2] != NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, "Das ist der Fehler");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, "Empty");
		}
	}	
}

FString APlayableCharacter::InventoryContent()
{
	
	//FString content;
	//for (int i = 0; i < 5; i++)
	//{
	//	if (inventory->inventoryList[i] != NULL)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "Ich bin jetzt in der if");
	//		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, inventory->inventoryList[0]->name);
	//		//return inventory->inventoryList[i]->name;
	//		//content += (inventory->inventoryList[i]->name + ", ");
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "Ich bin jetzt in der else");
	//		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, "Empty");
	//		//content += "Empty, ";
	//	}
	//	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, content);
	//}
	return inventory->inventoryList[0]->name;
	//return content;	
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

