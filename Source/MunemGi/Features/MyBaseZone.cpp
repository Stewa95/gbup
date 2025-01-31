// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseZone.h"


// Sets default values
AMyBaseZone::AMyBaseZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyBaseZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBaseZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

