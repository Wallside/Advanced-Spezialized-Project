// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UInteractableStaticMesh : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AInteractable*> contentReference;
};
