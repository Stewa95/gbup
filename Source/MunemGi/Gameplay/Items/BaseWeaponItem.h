// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class MUNEMGI_API ABaseWeaponItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ABaseWeaponItem();
	virtual void BeginPlay() override;
};
