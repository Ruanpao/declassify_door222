// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SlotActor.h"

#include "Components/TextRenderComponent.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "Kismet/GameplayStatics.h"


ASlotActor::ASlotActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	SlotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh"));
	SlotMesh->SetupAttachment(RootComponent);

	SlotText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SlotText"));
	SlotText->SetupAttachment(RootComponent);
	SlotText->SetHorizontalAlignment(EHTA_Center);
	SlotText->SetVerticalAlignment(EVRTA_TextCenter);
	SlotText->SetText(FText::FromString(TEXT("12")));
	
}

void ASlotActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlotActor::OnInteract_Implementation(AActor* Interactor)
{
    bool replaced=false;
    UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Start, Interactor: %s"), 
        Interactor ? *Interactor->GetName() : TEXT("NULL"));

    Adeclassify_doorCharacter * Player = Cast<Adeclassify_doorCharacter>(Interactor);

    if(!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("ASlotActor::OnInteract_Implementation - Interactor is not a valid player character"));
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Player found: %s"), 
        *Player->GetName());

    //回收
    if(CurrentPlate)
    {
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - RECYCLING MODE - CurrentPlate exists: %s"), 
            *CurrentPlate->GetName());
        
        Player->PickupStonePlate(CurrentPlate);
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Player picked up stone plate"));
        
        CurrentPlate->OnInteract_Implementation(Interactor);
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Called OnInteract on CurrentPlate"));
        
        OnPlateNumber.Broadcast(-AnsNumber);
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Broadcast OnPlateNumber with value: %d"), -AnsNumber);
        
        CurrentPlate = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - CurrentPlate set to nullptr"));
        replaced = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - No CurrentPlate to recycle"));
    }

    //放置
    if(Player->bHasStonePlate && Player->CurrentStonePlate && !replaced)
    {
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - PLACEMENT MODE"));
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Player->bHasStonePlate: %s"), 
            Player->bHasStonePlate ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Player->CurrentStonePlate: %s"), 
            Player->CurrentStonePlate ? *Player->CurrentStonePlate->GetName() : TEXT("NULL"));
        
        FVector SpawnLocation = GetActorLocation() + PlateOffset;
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Spawn location: %s, Rotation: %s"), 
            *SpawnLocation.ToString(), *GetActorRotation().ToString());
        
        AStonePlate * NewPlate = GetWorld()->SpawnActor<AStonePlate>(
            Player->CurrentStonePlate->GetClass(),
            SpawnLocation,
            GetActorRotation());

        if(NewPlate)
        {
            UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Successfully spawned new plate: %s"), 
                *NewPlate->GetName());
            
            NewPlate->SetPlateColor(Player->CurrentStonePlate->GetPlateColor());
            UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Set plate color"));
            
            CurrentPlate = NewPlate;
            UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - CurrentPlate set to new plate"));

            Player->bHasStonePlate = false;
            Player->CurrentStonePlate = nullptr;
            UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Reset player stone plate state"));
            
            OnPlateNumber.Broadcast(AnsNumber);
            UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Broadcast OnPlateNumber with value: %d"), AnsNumber);
            
            OnPlatePlaced.Broadcast(NewPlate->GetPlateColor());
            UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Broadcast OnPlatePlaced"));
            
            // 库存操作
            if (Player->InventoryComponent)
            {
                UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - InventoryComponent found"));
                UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - HeldItem Index: %d, ID: %s, Quantity: %d"), 
                    Player->InventoryComponent->HeldItem.Index, 
                    *Player->InventoryComponent->HeldItem.ID.ToString(), 
                    Player->InventoryComponent->HeldItem.Quantity);
                
                Player->InventoryComponent->DestroyAOldSlot(Player->InventoryComponent->HeldItem.Index);
                UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Called DestroyAOldSlot"));
                
                Player->InventoryComponent->UpdateHeldSlot(0);
                UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Called UpdateHeldSlot with index 0"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("ASlotActor::OnInteract_Implementation - Player InventoryComponent is NULL!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ASlotActor::OnInteract_Implementation - Failed to spawn new plate!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - Cannot place plate - conditions not met"));
        UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - bHasStonePlate: %s, CurrentStonePlate: %s"), 
            Player->bHasStonePlate ? TEXT("true") : TEXT("false"),
            Player->CurrentStonePlate ? TEXT("valid") : TEXT("NULL"));
    }

    UE_LOG(LogTemp, Warning, TEXT("ASlotActor::OnInteract_Implementation - End"));
			Player->bHasStonePlate = false;
			Player->CurrentStonePlate = nullptr;
			
			OnPlateNumber.Broadcast(AnsNumber);
			OnPlatePlaced.Broadcast(NewPlate->GetPlateColor());
			
			Player->InventoryComponent->DestroyAOldSlot(Player->InventoryComponent->HeldItem.Index);
			Player->InventoryComponent->UpdateHeldSlot(0);
		}
		if(PutSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,PutSound,GetActorLocation());
		}
	}
}

