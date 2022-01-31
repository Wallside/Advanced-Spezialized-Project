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
			hitObject->UnlockObject(this);
			hitObject->OpenAndClose();
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
	GetWorld()->LineTraceSingleByChannel(hit, playerLocation, endTrace, ECC_PhysicsBody, traceParams);

	return hit;
}

void APlayableCharacter::ChangeCrosshair() 
{
	FHitResult hit = ObjectToInteract();
	AInteractable* hitObject = Cast<AInteractable>(hit.Actor);
	UStaticMeshComponent* hitComponent = Cast<UStaticMeshComponent>(hit.GetComponent());


	if (hitObject && !hitComponent->GetName().Contains("Static"))
	{
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Ich das momentan nicht machen");
	}
	
}

void APlayableCharacter::TriggerTerrorRadius() 
{
	CalculatePostProcessSettingDifference(0);
	storymode->isMonsterActive = true;
}

void APlayableCharacter::SetPlayerCamera(UCameraComponent* camera) 
{
	playerCameraComponent = camera;
}

void APlayableCharacter::StopTerrorRadius() 
{
	CalculatePostProcessSettingDifference(1);
	isRecovering = true;
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
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Index out of bounce");
	}

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
}

