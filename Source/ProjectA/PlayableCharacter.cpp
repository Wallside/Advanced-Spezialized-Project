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
		else if (hitObject->locked)
		{
			hitObject->UnlockObject(this);
		}
		else 
		{
			hitObject->Interact(this);
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

void APlayableCharacter::ChangeCrosshair() 
{
	FHitResult hit = ObjectToInteract();
	AInteractable* hitObject = Cast<AInteractable>(hit.Actor);

	if (hitObject)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Interactive Crosshair");
		activeCrosshair = interactiveCrosshair;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Normal Crosshair");
		activeCrosshair = normalCrosshair;
	}
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

void APlayableCharacter::Defend()
{	
	StopTerrorRadius();
	storymode->isMonsterActive = false;
	storymode->isMonsterOnCooldown = true;
	storymode->MonsterCooldownTimerTick();
}

void APlayableCharacter::TriggerTerrorRadius() 
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Du bekommst eine Panikattacke");
	CalculatePostProcessSettingDifference();
}

void APlayableCharacter::SetPlayerCamera(UCameraComponent* camera) 
{
	playerCameraComponent = camera;
}

void APlayableCharacter::StopTerrorRadius() 
{
	activeSetting = normalSetting;
}

void APlayableCharacter::CalculatePostProcessSettingDifference() 
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

}

void APlayableCharacter::ApplyPostProcessSettingChanges() 
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

	playerCameraComponent->PostProcessSettings = activeSetting;
}

