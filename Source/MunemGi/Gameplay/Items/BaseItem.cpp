// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "Kismet/KismetMathLibrary.h"
#include "MunemGi/Character/BaseCharacter.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision box"));
	RootComponent = CollisionBox;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CollisionBox);
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CollisionBox);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::ItemOverlaped);
	
	if (!HasAuthority())
	{
		return;
	}
	
	if (bIsBaseItem)
	{
		if (UKismetMathLibrary::RandomBool())
		{
			const TSubclassOf<ABaseItem> RandomItemClassToSpawn = AllChildClasses[UKismetMathLibrary::RandomIntegerInRange(0, AllChildClasses.Num()-1)];
			ABaseItem* ItemRef = GetWorld()->SpawnActor<ABaseItem>(RandomItemClassToSpawn, GetActorTransform());
			if (ItemRef)
			{
				ItemRef->bIsBaseItem = false;
			}			
			Destroy();
		}
		else
		{
			Destroy();
		}
	}
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::ItemOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* CharacterRef = Cast<ABaseCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Display, TEXT("Is overlapped"));
		if (bIsHoldable)
		{
			UE_LOG(LogTemp, Display, TEXT("Is holdable"));
			SetActorEnableCollision(false);
			if (StaticMesh->GetStaticMesh())
			{
				StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ItemSocketName);
			CharacterRef->CurrentlyEquippedItem = ItemType;
		}
		// else
		// {
			// Add item to inventory 
		// }
	}
}
