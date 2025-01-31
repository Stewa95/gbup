// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlaneEndPoint.h"
#include "BasePlaneStartPoint.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlane.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MUNEMGI_API ABasePlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	USkeletalMeshComponent* PlaneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	UCameraComponent* PlaneCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	USpringArmComponent* SpringArm;
	
	UPROPERTY()
	FVector StartPoint;
	
	UPROPERTY()
	FVector EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BR Variables")
	float PlaneSpeed = 0.5f;

	AActor* EndPointActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BR Variables")
	bool bIsInFlight = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MovePlane();

	UFUNCTION(Server, Reliable)
	void SelectStartAndEnd();

	UFUNCTION()
	FVector InterpolatePlanePosition(FVector StartPosition, FVector EndPosition, FVector CurrentPosition, float Speed);
	
};
