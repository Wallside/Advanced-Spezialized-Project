// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"

/**
 * 
 */
class PROJECTA_API Item
{
public:

	Item();	
	~Item();

	Item(FString itemName, UImage* itemImage);

	FString name;
	UImage* image;
};
