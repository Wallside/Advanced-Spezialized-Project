// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateBrush.h"

/**
 * 
 */
class PROJECTA_API Item
{
public:

	Item();	
	~Item();

	Item(FString itemName, UTexture2D* itemImage);

	FString name;
	UTexture2D* image;
};
