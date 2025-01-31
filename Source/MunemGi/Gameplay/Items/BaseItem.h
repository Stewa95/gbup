// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UENUM(BlueprintType)
enum EItemType
{
	None,
	Weapon,
	Health,
	Throwable,
	Armor,
	Storage,
	Other
};

UCLASS()
class MUNEMGI_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Properties")
	bool bIsHoldable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Munem | Items")
	FName ItemSocketName = "";

	UFUNCTION()
	void ItemOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Munem | Items")
	bool bIsBaseItem = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Munem | Items")
	TEnumAsByte<EItemType> ItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Munem | Items")
	TArray<TSubclassOf<ABaseItem>> AllChildClasses;
};
