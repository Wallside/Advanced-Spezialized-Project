// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

Item::Item()
{

}

Item::~Item()
{

}

Item::Item(FString itemName, UImage* itemImage)
{
	name = itemName;
	image = itemImage;
}
