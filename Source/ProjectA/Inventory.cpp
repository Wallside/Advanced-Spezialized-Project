// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include <array>

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::AddItemToInventory(Item* item)
{
	int inventorySlot{0};
	bool isInventoryFull{false};
	do
	{
		if (inventorySlot >= 5)
		{
			isInventoryFull = true;
		}
		else if (inventoryList[inventorySlot] == NULL)
		{
			inventoryList[inventorySlot] = item;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Item hinzugefuegt");
			break;
		}			
		inventorySlot++;
	
	}while (!isInventoryFull);	
}

void Inventory::RemoveItemFromInventory(int index)
{
	inventoryList[index] = NULL;
}
