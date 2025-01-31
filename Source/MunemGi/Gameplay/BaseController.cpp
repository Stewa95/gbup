// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseController.h"

#include "MunemGi/Features/BasePlane.h"
#include "Net/UnrealNetwork.h"

void ABaseController::JumpFromPlane_Implementation()
{
	if (PlaneRef && bIsInPlane)
	{
		SetViewTarget(GetPawn());
		GetPawn()->SetActorLocation(PlaneRef->GetActorLocation());
		SetCharacterLocation(PlaneRef->GetActorLocation());
		bIsInPlane = false;
		return;
	}
	else
	{
		return;
	}
}

void ABaseController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// Super::GetLifetimeReplicatedProps(OutLifetime);
	DOREPLIFETIME(ABaseController, PlaneRef);
	DOREPLIFETIME(ABaseController, bIsInPlane);
}

void ABaseController::SetCharacterLocation_Implementation(FVector NewLocation)
{
	GetPawn()->SetActorLocation(NewLocation);
}
