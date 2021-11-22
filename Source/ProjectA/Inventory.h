// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Interactable.h"

/**
 * 
 */
class PROJECTA_API Inventory
{
public:
	Inventory();
	~Inventory();

	Item* inventoryList[5];


	void AddItemToInventory(Item* item);
};
