// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAirDrop.h"

// Sets default values
ABaseAirDrop::ABaseAirDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseAirDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAirDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

