// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SlotActor.h"

#include "Components/TextRenderComponent.h"
#include "declassify_door/declassify_doorCharacter.h"


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
	Adeclassify_doorCharacter * Player = Cast<Adeclassify_doorCharacter>(Interactor);

	if(!Player)
	{
		return;
	}
	//回收
	if( CurrentPlate)
	{
		Player->PickupStonePlate(CurrentPlate);
		CurrentPlate->Destroy();
		CurrentPlate = nullptr;
	}
	//放置
	if(Player->bHasStonePlate && Player->CurrentStonePlate)
	{
		AStonePlate * NewPlate = GetWorld()->SpawnActor<AStonePlate>(
			Player->CurrentStonePlate->GetClass(),
			GetActorLocation()+ PlateOffset,
			GetActorRotation());

		if(NewPlate)
		{
			NewPlate->SetPlateColor(Player->CurrentStonePlate->GetPlateColor());
			CurrentPlate = NewPlate;

			Player->bHasStonePlate = false;
			Player->CurrentStonePlate = nullptr;

			OnPlatePlaced.Broadcast(NewPlate->GetPlateColor());
			OnPlateNumber.Broadcast(AnsNumber);
			
			Player->InventoryComponent->DestroyAOldSlot(Player->InventoryComponent->HeldItem.Index);
		}
	}
}

