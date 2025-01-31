// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlane.h"

#include "VectorUtil.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasePlane::ABasePlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	PlaneMesh = CreateDefaultSubobject<USkeletalMeshComponent>("PlaneMesh");
	PlaneMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(PlaneMesh);

	PlaneCamera = CreateDefaultSubobject<UCameraComponent>("PlaneCamera");
	PlaneCamera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ABasePlane::BeginPlay()
{
	Super::BeginPlay();

	bIsInFlight = false;
}

void ABasePlane::SelectStartAndEnd_Implementation()
{
	TArray<AActor*> StartPointArray;
	TArray<AActor*> EndPointArray;
	UGameplayStatics::GetAllActorsOfClass(this, ABasePlaneStartPoint::StaticClass(), StartPointArray);
	UGameplayStatics::GetAllActorsOfClass(this, ABasePlaneEndPoint::StaticClass(), EndPointArray);

	StartPoint = StartPointArray[FMath::RandRange(0, StartPointArray.Num() - 1)]->GetActorLocation();
	EndPointActor = EndPointArray[FMath::RandRange(0, EndPointArray.Num() - 1)];
	EndPoint = EndPointActor->GetActorLocation();
	
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(StartPoint, EndPoint));
	SetActorLocation(StartPoint);
	bIsInFlight = true;

	//TESTING
	FTimerHandle LocalTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LocalTimerHandle, this, &ABasePlane::MovePlane, 0.01f, true, 0.3f);
}

// Called every frame
void ABasePlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetDistanceTo(EndPointActor) <= 100.0f && bIsInFlight)
	{
		Destroy();
	}
}

void ABasePlane::MovePlane()
{
	SetActorLocation(InterpolatePlanePosition(StartPoint, EndPoint, GetActorLocation(), 1000));
	// SetActorLocation(FMath::VInterpTo(StartPoint, EndPoint, GetWorld()->DeltaTimeSeconds, PlaneSpeed));
}

FVector ABasePlane::InterpolatePlanePosition(FVector StartPosition, FVector EndPosition, FVector CurrentPosition, float Speed)
{
	FVector TravelDistance = EndPosition - StartPosition;
	return CurrentPosition + TravelDistance / Speed;
}
