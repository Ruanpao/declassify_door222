// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/StonePlate.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "Kismet/GameplayStatics.h"

AStonePlate::AStonePlate()
{
	PrimaryActorTick.bCanEverTick = true;
    
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	PlateMesh->SetupAttachment(RootComponent);
}

void AStonePlate::BeginPlay()
{
	Super::BeginPlay();
}

void AStonePlate::OnInteract_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("=== AStonePlate::OnInteract_Implementation - START ==="));
	UE_LOG(LogTemp, Warning, TEXT("StonePlate ItemID: %s, bIsInSlot: %s"), 
		*ItemID.ToString(), bIsInSlot ? TEXT("true") : TEXT("false"));
    
	Adeclassify_doorCharacter* Player = Cast<Adeclassify_doorCharacter>(Interactor);
	if (Player && Player->InventoryComponent)
	{
		// 只有在不在槽位中时才能捡起
		if (!bIsInSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("Plate is not in slot, can be picked up"));
			UE_LOG(LogTemp, Warning, TEXT("Adding to inventory: %s"), *ItemID.ToString());
            
			// 将石板对应的物品ID添加到玩家库存
			Player->InventoryComponent->AddToInventory(ItemID, 1);
            
			UE_LOG(LogTemp, Warning, TEXT("Destroying stone plate actor"));
            
			if (PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
			}
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Plate is in slot, cannot be picked up directly"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player or InventoryComponent is null!"));
	}
    
	UE_LOG(LogTemp, Warning, TEXT("=== AStonePlate::OnInteract_Implementation - END ===\n"));
}