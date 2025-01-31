// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseController.generated.h"

class ABasePlane;

/**
 * 
 */
UCLASS()
class MUNEMGI_API ABaseController : public APlayerController
{
	GENERATED_BODY()


public:
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "BR Variables")
	ABasePlane* PlaneRef;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "BR Variables")
	bool bIsInPlane;

	UFUNCTION(Server, Reliable,BlueprintCallable, Category = "BR Functions")
	void JumpFromPlane();

	UFUNCTION(NetMulticast, Reliable, Category = "BR Functions")
	void SetCharacterLocation(FVector NewLocation);
};
