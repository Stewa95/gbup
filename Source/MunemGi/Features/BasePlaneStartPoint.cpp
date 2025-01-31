// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlaneStartPoint.h"

// Sets default values
ABasePlaneStartPoint::ABasePlaneStartPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasePlaneStartPoint::BeginPlay()
{
	Super::BeginPlay();
	FVector RandomStartLocation = FVector(GetActorLocation().X + FMath::RandRange(-200, 200), GetActorLocation().Y + FMath::RandRange(-200, 200), GetActorLocation().Z);
	SetActorLocation(RandomStartLocation);
}

// Called every frame
void ABasePlaneStartPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

