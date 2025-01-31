// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "BaseController.h"
#include "Kismet/GameplayStatics.h"
#include "MunemGi/Character/BaseCharacter.h"
#include "MunemGi/Features/BasePlane.h"
#include "MunemGi/Features/BaseZone.h"


void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentGetReadyTimer = MaxGetReadyTimer;
	CurrentWarmUpTimer = MaxWarmUpTimer;
	GetWorld()->GetTimerManager().SetTimer(WarmUpTimerHandle, this, &ABaseGameMode::DecreaseWarmUpTimer, 1.f, true);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseZone::StaticClass(), FindBaseZone);
	if (FindBaseZone[0] && Cast<ABaseZone>(FindBaseZone[0]))
	{
		BaseZoneRef = Cast<ABaseZone>(FindBaseZone[0]);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlane::StaticClass(), FindPlane);
	{
		if (FindPlane[0] && Cast<ABasePlane>(FindPlane[0]))
		{
			BasePlaneRef = Cast<ABasePlane>(FindPlane[0]);
		}
	}
}

void ABaseGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ABaseController* NewPlayerController = Cast<ABaseController>(NewPlayer);
	
	if (NewPlayer && NewPlayerController)
	{
		CurrentNumberOfPlayers++;
		AlivePlayerControllersList.Add(NewPlayerController);
		TotalPlayerControllersList.Add(NewPlayerController);
	}
}

void ABaseGameMode::DecreaseWarmUpTimer()
{
	CurrentWarmUpTimer--;
	
	if (CurrentWarmUpTimer <= 0 || CurrentNumberOfPlayers >= MinNumberOfPlayers)
	{
		CurrentWarmUpTimer = 0;
		GetWorld()->GetTimerManager().ClearTimer(WarmUpTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(GetReadyTimerHandle, this, &ABaseGameMode::DecreaseGetReadyTimer, 1.f, true);
	}
}

void ABaseGameMode::DecreaseGetReadyTimer()
{
	CurrentGetReadyTimer--;
	if (CurrentGetReadyTimer <= 0)
	{
		//Start the game
		GetWorld()->GetTimerManager().ClearTimer(GetReadyTimerHandle);
		if (BaseZoneRef && BasePlaneRef)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Zone is valid"));
			BaseZoneRef->InitializeZone();
			BasePlaneRef->SelectStartAndEnd();
			BasePlaneRef->MovePlane();
			MovePlayersToPlane();
		}
	}
}

void ABaseGameMode::MovePlayersToPlane()
{
	for (int i=0; i<TotalPlayerControllersList.Num(); i++)
	{
		if (TotalPlayerControllersList[i] && BasePlaneRef)
		{
			TotalPlayerControllersList[i]->SetViewTarget(BasePlaneRef);
			TotalPlayerControllersList[i]->PlaneRef = BasePlaneRef;
			TotalPlayerControllersList[i]->bIsInPlane = true;
			ABaseCharacter* PlayerActor = Cast<ABaseCharacter>(TotalPlayerControllersList[i]->GetCharacter());
			if (PlayerActor)
			{
				PlayerActor->SetIsInPlane(true); //Setting bIsInPlane on player actor for the jump functionality
			}
		}
	}
	return;
}
