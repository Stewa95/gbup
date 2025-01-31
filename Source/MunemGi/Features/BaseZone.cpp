// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseZone.h"

// Sets default values
ABaseZone::ABaseZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	
	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Zone mesh"));
	SetRootComponent(ZoneMesh);	
}

void ABaseZone::TimelineForShrink(float Value)
{
	
}

// Called when the game starts or when spawned
void ABaseZone::BeginPlay()
{
	Super::BeginPlay();
	if(CurveFloat)
	{
		FOnTimelineFloat TimelineFloat;
		TimelineFloat.BindUFunction(this,FName("TimelineForShrink"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineFloat);
	}

	ScaleInBetweenZones = ZoneMesh->GetComponentScale();
	float RandomXPosition = FMath::FRand() *(GetActorLocation().X + ((ZoneInfoArray[CurrentStepNumber ].TargetDecreasedScale * 40))/2);
	float RandomYPosition = FMath::FRand() * (GetActorLocation().Y + ((ZoneInfoArray[CurrentStepNumber ].TargetDecreasedScale * 40))/2);
	RandomXPosition = FMath::RandRange(-1, 1) * RandomXPosition;
	RandomYPosition = FMath::RandRange(-1, 1) * RandomYPosition;
	RandomZoneLocation = FVector(RandomXPosition, RandomYPosition , GetActorLocation().Z);
}

void ABaseZone::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ZoneMesh->SetWorldScale3D(FVector(Radius, Radius, Height));
}

void ABaseZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void ABaseZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABaseZone::InitializeZone()
{
	UE_LOG(LogTemp, Display, TEXT("Zone will start in %f seconds"), ZoneMovementDelay);
	GetWorld()->GetTimerManager().SetTimer(ZoneInitializationTimerHandle, this, &ABaseZone::Server_StartMovingZone, ZoneMovementDelay, false);
}

float ABaseZone::InterpolateZoneScale(FVector StartScale, FVector EndScale, FVector CurrentScale, float MovementTime)
{
	float  TargetScaleDecrease = StartScale.X - EndScale.X;
	return TargetScaleDecrease / MovementTime;
}

void ABaseZone::Sever_CalculateZoneMovementTimer_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(ZoneMovingTimerHandle);
	ScaleInBetweenZones = ZoneMesh->GetComponentScale();
	if (CurrentStepNumber < ZoneInfoArray.Num())
	{
		float RandomXPosition = FMath::FRand() *(GetActorLocation().X + ((ZoneInfoArray[CurrentStepNumber ].TargetDecreasedScale * 40))/2);
		float RandomYPosition = FMath::FRand() * (GetActorLocation().Y + ((ZoneInfoArray[CurrentStepNumber ].TargetDecreasedScale * 40))/2);
		RandomXPosition = FMath::RandRange(-1, 1) * RandomXPosition;
		RandomYPosition = FMath::RandRange(-1, 1) * RandomYPosition;
		RandomZoneLocation = FVector(RandomXPosition, RandomYPosition , GetActorLocation().Z);
	}
	UE_LOG(LogTemp, Error, TEXT("%d. zone location: X: %f, Y: %f"),CurrentStepNumber, GetActorLocation().X, GetActorLocation().Y);
}

void ABaseZone::Server_RecallServerMovement_Implementation()
{
	Server_StartMovingZone();
}

void ABaseZone::Server_TimerStartMovingZone_Implementation()
{
	FVector LocalScale;
	float LocalScaleDecrease = InterpolateZoneScale(ScaleInBetweenZones,
	FVector( ZoneInfoArray[CurrentStepNumber-1].TargetDecreasedScale,ZoneInfoArray[CurrentStepNumber-1].TargetDecreasedScale, ZoneMesh->GetComponentScale().Z),
	ZoneMesh->GetComponentScale(),
			ZoneInfoArray[CurrentStepNumber - 1].MovementDuration);
	LocalScaleDecrease *= ZoneDecreaseRate;
	LocalScale = FVector(ZoneMesh->GetComponentScale().X - LocalScaleDecrease, ZoneMesh->GetComponentScale().Y - LocalScaleDecrease, ZoneMesh->GetComponentScale().Z);
	
	FVector LocalNewZoneLocation = (RandomZoneLocation / ZoneInfoArray[CurrentStepNumber-1].MovementDuration)*ZoneDecreaseRate;
	FVector LocalNewLocation = GetActorLocation() - LocalNewZoneLocation;
	
	Multi_StartMovingZone(LocalScale, LocalNewLocation);
}

void ABaseZone::Multi_StartMovingZone_Implementation(const FVector NewScale, FVector NewLocation)
{
	if (ZoneMesh)
	{
		ZoneMesh->SetRelativeScale3D(NewScale);
		SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Zone mesh not valid"));
	}
}

void ABaseZone::Server_StartMovingZone_Implementation()
{
	if (CurrentStepNumber < ZoneInfoArray.Num())
	{
		GetWorld()->GetTimerManager().SetTimer(ZoneMovingTimerHandle, this, &ABaseZone::Server_TimerStartMovingZone, ZoneDecreaseRate, true);
		GetWorld()->GetTimerManager().SetTimer(CalculateMovingTimerHandle, this, &ABaseZone::Sever_CalculateZoneMovementTimer, ZoneInfoArray[CurrentStepNumber].MovementDuration, false);
		float TotalTime = ZoneInfoArray[CurrentStepNumber].HoldTime + ZoneInfoArray[CurrentStepNumber].MovementDuration;
		GetWorld()->GetTimerManager().SetTimer(RecallTimerHandle, this, &ABaseZone::Server_RecallServerMovement, TotalTime, false);
		CurrentStepNumber++;
	}
}
