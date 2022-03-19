// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
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

	activeCrosshair = normalCrosshair;

	storymode = Cast<AStory_GameMode>(GetWorld()->GetAuthGameMode());

	playerCameraComponent->PostProcessSettings = activeSetting;

	

	
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
	UStaticMeshComponent* hitComponent = Cast<UStaticMeshComponent>(hit.GetComponent());

	if (hitObject && !hitComponent->GetName().Contains("Static"))
	{
		if (hitObject->incomplete)
		{			
			hitObject->CompleteObject(this);
		}
		else if (hitObject->locked)
		{
			hitObject->UnlockObject(this, hitComponent);
		}
		else 
		{
			hitObject->Interact(this, hitComponent);
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
	GetWorld()->LineTraceSingleByChannel(hit, playerLocation, endTrace, ECC_PhysicsBody, traceParams);

	return hit;
}

void APlayableCharacter::ChangeCrosshair() 
{
	FHitResult hit = ObjectToInteract();
	AInteractable* hitObject = Cast<AInteractable>(hit.Actor);
	UStaticMeshComponent* hitComponent = Cast<UStaticMeshComponent>(hit.GetComponent());


	if (hitObject && hitObject->objectType != ObjectType::None && !hitComponent->GetName().Contains("Static"))
	{
		activeCrosshair = interactiveCrosshair;
	}
	else
	{
		activeCrosshair = normalCrosshair;
	}
}

void APlayableCharacter::SetItemNameInIndex(FString itemName, int index)
{
	inventory->inventoryList[index]->name = itemName;
}

FString APlayableCharacter::GetItemNameInIndex(int index)
{
	if (inventory->inventoryList[index] != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Name got");
		return inventory->inventoryList[index]->name;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Name NULL");
		return "";
	}
}

void APlayableCharacter::SetItemIconInIndex(UTexture2D* itemIcon, int index)
{
	inventory->inventoryList[index]->image = itemIcon;
}


UTexture2D* APlayableCharacter::GetItemIconInIndex(int index)
{
	if (inventory->inventoryList[index] != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Icon got");
		return inventory->inventoryList[index]->image;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Icon NULL");
		return NULL;
	}
}

void APlayableCharacter::InputDefend()
{	
	if (storymode->isMonsterActive)
	{
		for (int i = 0; i < 5; i++)
		{
			if (inventory->inventoryList[i] != NULL)
			{

				if (inventory->inventoryList[i]->name == "Tape Recorder")
				{
					for (int j = 0; j < 5;j++)
					{
						if (inventory->inventoryList[j] != NULL)
						{
							if (inventory->inventoryList[j]->name == "Tapes")
							{
								StopTerrorRadius();
								storymode->isMonsterActive = false;
								storymode->isMonsterOnCooldown = true;
								storymode->monsterKillCountdown = 15;
								storymode->MonsterCooldownTimerTick();
								inventory->RemoveItemFromInventory(j);
								break;
							}
						}
						else if (i == 4)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Ich habe keine Kassetten im Inventar");
						}
					}
					
				}
			}
			else if (i == 4)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Ich habe kein nutzbares Item im Inventar");
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Ich kann das momentan nicht machen");
	}	
}

void APlayableCharacter::Defend()
{
	if (storymode->isMonsterActive)
	{
		StopTerrorRadius();
		storymode->isMonsterActive = false;
		storymode->isMonsterOnCooldown = true;
		storymode->monsterKillCountdown = 15;
		storymode->MonsterCooldownTimerTick();	
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Ich kann das momentan nicht machen");
	}
}

void APlayableCharacter::TriggerTerrorRadius() 
{
	CalculatePostProcessSettingDifference(0);
	storymode->isMonsterActive = true;
	ToggleMonsterSound();
}

void APlayableCharacter::SetPlayerCamera(UCameraComponent* camera) 
{
	playerCameraComponent = camera;
}

void APlayableCharacter::StopTerrorRadius() 
{
	CalculatePostProcessSettingDifference(1);
	isRecovering = true;
	ToggleMonsterSound();
	RecoveryTimerTick();
}

void APlayableCharacter::CalculatePostProcessSettingDifference(int index) 
{
	if (index == 0)
	{
		changeValues.chromaticAberrationIntensitiy = (terrorSetting.SceneFringeIntensity - normalSetting.SceneFringeIntensity) / (storymode->monsterKillCountdown * 10);

		changeValues.imageEffectsVignetteIntensity = (terrorSetting.VignetteIntensity - normalSetting.VignetteIntensity) / (storymode->monsterKillCountdown * 10);
		changeValues.imageEffectsGrainJitter = (terrorSetting.GrainJitter - normalSetting.GrainJitter) / (storymode->monsterKillCountdown * 10);
		changeValues.imageEffectsGrainIntensity = (terrorSetting.GrainIntensity - normalSetting.GrainIntensity) / (storymode->monsterKillCountdown * 10);

		changeValues.colorGradingGlobalSaturationR = (normalSetting.ColorSaturation.X - terrorSetting.ColorSaturation.X) / (storymode->monsterKillCountdown * 10);
		changeValues.colorGradingGlobalSaturationG = (normalSetting.ColorSaturation.Y - terrorSetting.ColorSaturation.Y) / (storymode->monsterKillCountdown * 10);
		changeValues.colorGradingGlobalSaturationB = (normalSetting.ColorSaturation.Z - terrorSetting.ColorSaturation.Z) / (storymode->monsterKillCountdown * 10);
		changeValues.colorGradingGlobalSaturationY = (normalSetting.ColorSaturation.W - terrorSetting.ColorSaturation.W) / (storymode->monsterKillCountdown * 10);

		changeValues.colorGradingGlobalOffsetR = (normalSetting.ColorOffset.X - terrorSetting.ColorOffset.X) / (storymode->monsterKillCountdown * 10);
		changeValues.colorGradingGlobalOffsetG = (normalSetting.ColorOffset.Y - terrorSetting.ColorOffset.Y) / (storymode->monsterKillCountdown * 10);
		changeValues.colorGradingGlobalOffsetB = (normalSetting.ColorOffset.Z - terrorSetting.ColorOffset.Z) / (storymode->monsterKillCountdown * 10);
		changeValues.colorGradingGlobalOffsetY = (normalSetting.ColorOffset.W - terrorSetting.ColorOffset.W) / (storymode->monsterKillCountdown * 10);

		changeValues.monsterIntensity = 5.0f / (storymode->monsterKillCountdown * 10);
		
	}
	else if (index == 1)
	{
		changeValues.chromaticAberrationIntensitiy = (activeSetting.SceneFringeIntensity - normalSetting.SceneFringeIntensity) / (recoveryTime * 10);

		changeValues.imageEffectsVignetteIntensity = (activeSetting.VignetteIntensity - normalSetting.VignetteIntensity) / (recoveryTime * 10);
		changeValues.imageEffectsGrainJitter = (activeSetting.GrainJitter - normalSetting.GrainJitter) / (recoveryTime * 10);
		changeValues.imageEffectsGrainIntensity = (activeSetting.GrainIntensity - normalSetting.GrainIntensity) / (recoveryTime * 10);

		changeValues.colorGradingGlobalSaturationR = (normalSetting.ColorSaturation.X - activeSetting.ColorSaturation.X) / (recoveryTime * 10);
		changeValues.colorGradingGlobalSaturationG = (normalSetting.ColorSaturation.Y - activeSetting.ColorSaturation.Y) / (recoveryTime * 10);
		changeValues.colorGradingGlobalSaturationB = (normalSetting.ColorSaturation.Z - activeSetting.ColorSaturation.Z) / (recoveryTime * 10);
		changeValues.colorGradingGlobalSaturationY = (normalSetting.ColorSaturation.W - activeSetting.ColorSaturation.W) / (recoveryTime * 10);

		changeValues.colorGradingGlobalOffsetR = (normalSetting.ColorOffset.X - activeSetting.ColorOffset.X) / (recoveryTime * 10);
		changeValues.colorGradingGlobalOffsetG = (normalSetting.ColorOffset.Y - activeSetting.ColorOffset.Y) / (recoveryTime * 10);
		changeValues.colorGradingGlobalOffsetB = (normalSetting.ColorOffset.Z - activeSetting.ColorOffset.Z) / (recoveryTime * 10);
		changeValues.colorGradingGlobalOffsetY = (normalSetting.ColorOffset.W - activeSetting.ColorOffset.W) / (recoveryTime * 10);

		changeValues.monsterIntensity = 4.0f / (recoveryTime * 10);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Index out of bounce");
	}

	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, "Monster Intensity: " + FString::SanitizeFloat(10 / storymode->monsterKillCountdown * 10));
	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "Chromatic Aberration: " + FString::SanitizeFloat(changeValues.chromaticAberrationIntensitiy));
	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "VignetteIntesity: " + FString::SanitizeFloat(changeValues.imageEffectsVignetteIntensity));
	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "GrainJitter: " + FString::SanitizeFloat(changeValues.imageEffectsGrainJitter));
	//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "GrainIntesity: " + FString::SanitizeFloat(changeValues.imageEffectsGrainIntensity));

}

void APlayableCharacter::ApplyPostProcessSettingChanges(int index) 
{
	if (index == 0)
	{
		activeSetting.SceneFringeIntensity += changeValues.chromaticAberrationIntensitiy;

		activeSetting.VignetteIntensity += changeValues.imageEffectsVignetteIntensity;
		activeSetting.GrainJitter += changeValues.imageEffectsGrainJitter;
		activeSetting.GrainIntensity += changeValues.imageEffectsGrainIntensity;

		activeSetting.ColorSaturation.X -= changeValues.colorGradingGlobalSaturationR;
		activeSetting.ColorSaturation.Y -= changeValues.colorGradingGlobalSaturationG;
		activeSetting.ColorSaturation.Z -= changeValues.colorGradingGlobalSaturationB;
		activeSetting.ColorSaturation.W -= changeValues.colorGradingGlobalSaturationY;

		activeSetting.ColorOffset.X -= changeValues.colorGradingGlobalOffsetR;
		activeSetting.ColorOffset.Y -= changeValues.colorGradingGlobalOffsetG;
		activeSetting.ColorOffset.Z -= changeValues.colorGradingGlobalOffsetB;
		activeSetting.ColorOffset.W -= changeValues.colorGradingGlobalOffsetY;

		monsterIntensity += changeValues.monsterIntensity;

		GetMonsterIntensity();
	}
	else if (index == 1)
	{
		activeSetting.SceneFringeIntensity -= changeValues.chromaticAberrationIntensitiy;

		activeSetting.VignetteIntensity -= changeValues.imageEffectsVignetteIntensity;
		activeSetting.GrainJitter -= changeValues.imageEffectsGrainJitter;
		activeSetting.GrainIntensity -= changeValues.imageEffectsGrainIntensity;

		activeSetting.ColorSaturation.X += changeValues.colorGradingGlobalSaturationR;
		activeSetting.ColorSaturation.Y += changeValues.colorGradingGlobalSaturationG;
		activeSetting.ColorSaturation.Z += changeValues.colorGradingGlobalSaturationB;
		activeSetting.ColorSaturation.W += changeValues.colorGradingGlobalSaturationY;

		activeSetting.ColorOffset.X += changeValues.colorGradingGlobalOffsetR;
		activeSetting.ColorOffset.Y += changeValues.colorGradingGlobalOffsetG;
		activeSetting.ColorOffset.Z += changeValues.colorGradingGlobalOffsetB;
		activeSetting.ColorOffset.W += changeValues.colorGradingGlobalOffsetY;

		monsterIntensity -= changeValues.monsterIntensity;
	}

	playerCameraComponent->PostProcessSettings = activeSetting;
}

void APlayableCharacter::RecoveryTimerTick() 
{
	OnRecoveryTimerTick();
}

void APlayableCharacter::NormalizePostProcessingSettings() 
{
	activeSetting = normalSetting;

	playerCameraComponent->PostProcessSettings = activeSetting;

	monsterIntensity = 1;
}

void APlayableCharacter::CollectSound() 
{
	PlayCollectSound();
}

void APlayableCharacter::QueueRoomSound(FString enteredRoom)
{
	roomName = enteredRoom;
	RefreshRoomSound();
}

void APlayableCharacter::SafeInspectableObject(AInteractable* inspectableObject) 
{
	inspectedObject = inspectableObject;
}

AStory_GameMode* APlayableCharacter::GetStoryMode()
{
	return storymode;
}

void APlayableCharacter::ViewChanges()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, "Monster Intensity: " + FString::SanitizeFloat(5.0f / storymode->monsterKillCountdown * 10));
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "Chromatic Aberration: " + FString::SanitizeFloat((terrorSetting.SceneFringeIntensity - normalSetting.SceneFringeIntensity) / (storymode->monsterKillCountdown * 10)));
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "VignetteIntesity: " + FString::SanitizeFloat(changeValues.imageEffectsVignetteIntensity));
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "GrainJitter: " + FString::SanitizeFloat(changeValues.imageEffectsGrainJitter));
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, "GrainIntesity: " + FString::SanitizeFloat(changeValues.imageEffectsGrainIntensity));
}

void APlayableCharacter::TriggerAudioEvent(AInteractable* object)
{
	PlayAudioEvent(object);
}

void APlayableCharacter::TriggerPauseGame()
{
	PauseGame();
	storymode->isGamePaused = true;
}

void APlayableCharacter::TriggerContinueGame()
{
	ContinueGame();
	storymode->isGamePaused = false;
}

void APlayableCharacter::CreateItemToAddToInventory(FString itemName, UTexture2D* itemIcon)
{
	if (itemName != "" && itemIcon != NULL)
	{
		Item* newItem = new Item(itemName, itemIcon);
		inventory->AddItemToInventory(newItem);
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, itemName);
	}
}

