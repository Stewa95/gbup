// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBaseZone.generated.h"

UCLASS()
class MUNEMGI_API AMyBaseZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyBaseZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Zone properies")
	UStaticMeshComponent* ZoneMesh;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
