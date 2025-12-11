// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/KeyActor.h"

#include "declassify_door/declassify_doorCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


AKeyActor::AKeyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComponent);
	KeyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KeyMesh->SetGenerateOverlapEvents(true);
}


void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyActor::OnInteract_Implementation(AActor* Interactor)
{
	if(!Interactor)
	{
		return;
	}

	if(ACharacter * Character = Cast<ACharacter>(Interactor))
	{
		if(Character->IsA(Adeclassify_doorCharacter::StaticClass()))
		{
			Adeclassify_doorCharacter* MyCharacter = Cast<Adeclassify_doorCharacter>(Character);
			if(MyCharacter->InventoryComponent)
			{
				MyCharacter->InventoryComponent->AddToInventory(FName("1"),1);
			}
		}
	}
	
	
	if (OnKeyPickedUp.IsBound())
	{
		OnKeyPickedUp.Broadcast();
	}
	
	TArray<AActor*> AllKeys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeyActor::StaticClass(), AllKeys);
    
	for (AActor* Key : AllKeys)
	{
		if (Key)
		{
			if(PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
			}
			Key->Destroy();
		}
	}
}

