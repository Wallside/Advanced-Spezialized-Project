// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

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
		if (inventorySlot >= 7)
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
	SortInventory();
}

void Inventory::SortInventory() 
{
	for (int i = 0; i < 7; i++)
	{
		if ((i + 1) >= 7)
		{
			break;
		}
		else
		{
			if (inventoryList[i] == NULL && inventoryList[i + 1] != NULL)
			{
				inventoryList[i] = inventoryList[i + 1];
				inventoryList[i + 1] = NULL;
			}
		}
	}
}
