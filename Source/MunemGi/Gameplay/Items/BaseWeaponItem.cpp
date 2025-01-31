// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponItem.h"

ABaseWeaponItem::ABaseWeaponItem()
{
	ItemType = EItemType::Weapon;
	bIsBaseItem = false;
	bIsHoldable = true;
	ItemSocketName = "WeaponSocket";
}

void ABaseWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}
