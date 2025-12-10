// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/StonePlate.h"

#include "declassify_door/declassify_doorCharacter.h"
#include "Kismet/GameplayStatics.h"


AStonePlate::AStonePlate()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	// 创建石板网格
	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	PlateMesh->SetupAttachment(RootComponent);
}

void AStonePlate::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStonePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStonePlate::OnInteract_Implementation(AActor* Interactor)
{
	if(!Interactor)
	{
		return;
	}
	if(ACharacter* Character = Cast<ACharacter>(Interactor))
	{
		if(Character->IsA(Adeclassify_doorCharacter::StaticClass()))
		{
			Adeclassify_doorCharacter* MyCharacter = Cast<Adeclassify_doorCharacter>(Character);
			if(MyCharacter->InventoryComponent)
			{
				if(PlateColor==FLinearColor::Red)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("7"),1);
				}
				else if(PlateColor==FLinearColor::Green)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("8"),1);
				}
				else if(PlateColor==FLinearColor::Blue)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("9"),1);
				}
				else if(PlateColor==FLinearColor::White)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("10"),1);
				}
				
				MyCharacter->PickupStonePlate(this);
			}
			
			if(PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this,PickupSound,GetActorLocation());
			}
			Destroy();
		}
	}
}

void AStonePlate::SetPlateColor(FLinearColor NewColor)
{
	PlateColor = NewColor;
}

