// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

class ABaseController;
class ABasePlane;
class ABaseZone;

/**
 * 
 */
UCLASS()
class MUNEMGI_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	//Waiting for players to connect
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "BR Variables")
	float CurrentWarmUpTimer;

	//Delay between all players connecting and starting the game
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "BR Variables")
	float CurrentGetReadyTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float MaxWarmUpTimer = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float MaxGetReadyTimer = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	int MinNumberOfPlayers = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	int CurrentNumberOfPlayers = 0;
	
private:
	//Called when game starts
	virtual void BeginPlay() override;
	//Called when new player connects
	virtual void OnPostLogin(AController* NewPlayer) override;

	UFUNCTION()
	void DecreaseWarmUpTimer();

	UFUNCTION()
	void DecreaseGetReadyTimer();

	UFUNCTION()
	void MovePlayersToPlane();

	FTimerHandle WarmUpTimerHandle;
	FTimerHandle GetReadyTimerHandle;

	UPROPERTY()
	ABaseZone* BaseZoneRef;

	UPROPERTY()
	ABasePlane* BasePlaneRef;

	TArray<AActor*> FindBaseZone;
	TArray<AActor*> FindPlane;

	//Players list
	TArray<ABaseController*> TotalPlayerControllersList;
	TArray<ABaseController*> AlivePlayerControllersList;
	TArray<ABaseController*> DeadPlayerControllersList;
};
