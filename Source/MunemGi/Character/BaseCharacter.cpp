// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MunemGi/Gameplay/BaseController.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera spring arm"));
	PlayerCameraSpringArm->SetupAttachment(GetMesh(), TEXT("head"));

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player camera"));
	PlayerCamera->SetupAttachment(PlayerCameraSpringArm);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		return;
	}
	FTimerHandle ZoneTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ZoneTimerHandle, this, &ABaseCharacter::DamagePlayerIfNotInZone, 0.1f, true);
	
	SetTPCameraSrv();
	UE_LOG(LogTemp, Warning, TEXT("Set to TP camera on begin play"));
	StopSprint();
	UE_LOG(LogTemp, Warning, TEXT("Set to walking"));
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName(*MoveForwardKey), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName(*MoveRightKey), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName(*LookUpKey), this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName(*TurnKey), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAction(FName(*SprintKey),IE_Pressed, this, &ABaseCharacter::Sprint);
	PlayerInputComponent->BindAction(FName(*SprintKey),IE_Released, this, &ABaseCharacter::StopSprint);
	PlayerInputComponent->BindAction(FName(*JumpKey),IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(FName(*CameraModeKey),IE_Pressed, this, &ABaseCharacter::ChangeCameraMode);
}

void ABaseCharacter::SetIsInPlane(bool bLocalIsInPlane)
{
	bIsInPlane = bLocalIsInPlane;
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	Mlt_PlayerLanded();
}

void ABaseCharacter::MoveForward(float AxisVal)
{
	AddMovementInput(GetActorForwardVector()*AxisVal);
}

void ABaseCharacter::MoveRight(float AxisVal)
{
	AddMovementInput(GetActorRightVector()*AxisVal);
}

void ABaseCharacter::LookUp(float AxisVal)
{
	AddControllerPitchInput(AxisVal);
}

void ABaseCharacter::Turn(float AxisVal)
{
	AddControllerYawInput(AxisVal);
}

void ABaseCharacter::Jump()
{
	if (bIsInPlane)
	{
		ABaseController* PlayerController = Cast<ABaseController>(GetController());
		if (PlayerController)
		{
			PlayerController->JumpFromPlane();
			Mlt_SkydiveForPlayer();
		}
	}
	else
	{
		Super::Jump();
	}
}

void ABaseCharacter::Sprint()
{
	UCharacterMovementComponent* MovementComponentPointer = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if(MovementComponentPointer)
	{
		if (!bPlayerIsSprinting)
		{
			bPlayerIsSprinting = true;	
			MovementComponentPointer->MaxWalkSpeed = SprintSpeed;
			UE_LOG(LogTemp, Warning, TEXT("Sprinting"));
		}
		// else
		// {
		// 	bPlayerIsSprinting = false;	
		// 	MovementComponentPointer->MaxWalkSpeed = WalkSpeed;
		// 	UE_LOG(LogTemp, Warning, TEXT("Not sprinting"));
		// }
	}
}

void ABaseCharacter::StopSprint()
{
	UCharacterMovementComponent* MovementComponentPointer = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if(MovementComponentPointer)
	{
		bPlayerIsSprinting = false;	
		MovementComponentPointer->MaxWalkSpeed = WalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Not sprinting on stop sprinting"));
	}
}

void ABaseCharacter::ChangeCameraMode()
{
	if(bIsTPCamera)
	{
		SetFPCameraSrv();
	}
	else
	{
		SetTPCameraSrv();
	}
}

void ABaseCharacter::SetTPCameraClt()
{
	PlayerCameraSpringArm->TargetArmLength = 250.0f;
	bIsTPCamera = true;
}

void ABaseCharacter::SetFPCameraClt()
{
	PlayerCameraSpringArm->TargetArmLength = 0.0f;
	bIsTPCamera = false;
}

void ABaseCharacter::SetFPCameraSrv_Implementation()
{
	PlayerCameraSpringArm->TargetArmLength = 0.0f;
	bIsTPCamera = false;
	SetFPCameraClt();
}

void ABaseCharacter::SetTPCameraSrv_Implementation()
{
	PlayerCameraSpringArm->TargetArmLength = 250.0f;
	bIsTPCamera = true;
	UE_LOG(LogTemp, Warning, TEXT("Set to TP camera"));
	SetTPCameraClt();
}

void ABaseCharacter::Mlt_PlayerLanded_Implementation()
{
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.75f;
}

void ABaseCharacter::Mlt_SkydiveForPlayer_Implementation()
{
	GetCharacterMovement()->Velocity = UKismetMathLibrary::VLerp(FVector(), GetVelocity(), 0.1f);
	GetCharacterMovement()->AirControl = 0.7f;
	GetCharacterMovement()->GravityScale = 0.1f;
}

void ABaseCharacter::Mlt_PlayerDeath_Implementation()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ABaseCharacter::DamagePlayerIfNotInZone_Implementation()
{
	if (!bIsInZone && bIsInZone)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is out side of zone"));
		CurrentHealth = FMath::Clamp(CurrentHealth - 0.1f,0.0f,100.0f);
		if (CurrentHealth <= 0.0f)
		{
			bIsAlive = false;
			Mlt_PlayerDeath();
		}
		CltDamagePlayerIfNotInZoneLocally();
	}
}

void ABaseCharacter::CltDamagePlayerIfNotInZoneLocally_Implementation()
{
	OnPlayerDamaged();
}


void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, bIsInZone);
	DOREPLIFETIME(ABaseCharacter, bPlayerIsSprinting);
	DOREPLIFETIME(ABaseCharacter, bIsTPCamera);
	DOREPLIFETIME(ABaseCharacter, CurrentHealth);
	DOREPLIFETIME(ABaseCharacter, bIsAlive);
	DOREPLIFETIME(ABaseCharacter, CurrentlyEquippedItem);
}
