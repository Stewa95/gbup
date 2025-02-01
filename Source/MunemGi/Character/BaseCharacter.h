// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MunemGi/Gameplay/Items/BaseItem.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MUNEMGI_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Public Variables

	//Define input key for move forward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlls")
	FString MoveForwardKey;

	//Define input key for move right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlls")
	FString MoveRightKey;

	//Define input key for looking up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlls")
	FString LookUpKey;

	//Define input key for turning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlls")
	FString TurnKey;

	//Define input key for jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlls")
	FString JumpKey;

	//Define input key for sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controlls")
	FString SprintKey;

	//Define input key for CameraMode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString CameraModeKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
	TEnumAsByte<EItemType> CurrentlyEquippedItem = EItemType::None;

	void SetIsInPlane(bool bLocalIsInPlane);

	virtual void Landed(const FHitResult& Hit) override;

protected:
	void MoveForward(float AxisVal);
	void MoveRight(float AxisVal);
	void LookUp(float AxisVal);
	void Turn(float AxisVal);
	void Jump();
	void Sprint();
	void StopSprint();
	void ChangeCameraMode();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAcces = true), Replicated)
	bool bPlayerIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* PlayerCameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone", Replicated)
	bool bIsInZone = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	bool bIsAlive = true;
	
	bool bIsInPlane = false;

	UFUNCTION(Server, Unreliable)
	void DamagePlayerIfNotInZone();

	UFUNCTION(NetMulticast, Unreliable)
	void Mlt_PlayerDeath();
	
	UFUNCTION(Client, Unreliable)
	void CltDamagePlayerIfNotInZoneLocally();

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnPlayerDamaged();

    UFUNCTION(NetMulticast, Unreliable)
	void Mlt_SkydiveForPlayer();

	UFUNCTION(NetMulticast, Unreliable)
	void Mlt_PlayerLanded();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsTPCamera = false;

	UFUNCTION(Server, Unreliable)
	void SetTPCameraSrv();

	void SetTPCameraClt();
	
	UFUNCTION(Server, Unreliable)
	void SetFPCameraSrv();

	void SetFPCameraClt();
};
