// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseZone.generated.h"

USTRUCT(BlueprintType)
struct FZoneInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float HoldTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float MovementDuration;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	// float ScaleToDecrease = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float TargetDecreasedScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float Damage;
};

class UCurveFloat;

UCLASS()
class MUNEMGI_API ABaseZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseZone();

	//Timeline function
	UFUNCTION()
	void TimelineForShrink(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	TArray<FZoneInfo> ZoneInfoArray;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	// float Height = 1000.0f;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	// float Radius = 1000.0f;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Overrinde construction graph 
	virtual void OnConstruction(const FTransform& Transform) override;

	//Override replication function
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BR Variables")
	UStaticMeshComponent* ZoneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float Height = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float Radius = 1000.0f;

	//Timeline functions
	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "BR Variables")
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "BR Variables")
	float ZoneMovementDelay = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BR Variables")
	int32 CurrentStepNumber;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BR Variables")
	float ZoneDecreaseRate = 0.05f;

	UPROPERTY(EditAnywhere, Category = "BR Variables")
	TSubclassOf<ABaseZone> ZonePreviewClass;
	
	FVector ScaleInBetweenZones;
	FVector RandomZoneLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Server_StartMovingZone();

	UFUNCTION(Server, Reliable)
	void Server_RecallServerMovement();

	UFUNCTION(Server, Reliable)
	void Sever_CalculateZoneMovementTimer();

	void InitializeZone();

	UFUNCTION(Server, Reliable)
	void Server_TimerStartMovingZone();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_StartMovingZone(const FVector NewScale, FVector NewLocation);

	UFUNCTION()
	float InterpolateZoneScale(FVector StartScale, FVector EndScale, FVector CurrentScale, float MovementTime);

	FTimerHandle ZoneMovingTimerHandle;
	FTimerHandle RecallTimerHandle;
	FTimerHandle CalculateMovingTimerHandle;
	FTimerHandle ZoneInitializationTimerHandle;

	bool DoOnce = true;
};
