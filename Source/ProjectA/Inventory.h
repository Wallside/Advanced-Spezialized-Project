// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"

/**
 * 
 */
class PROJECTA_API Inventory
{

public:
	Inventory();
	~Inventory();

	Item* inventoryList[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};


	void AddItemToInventory(Item* item);

	void RemoveItemFromInventory(int index);

private:

	void SortInventory();
};
