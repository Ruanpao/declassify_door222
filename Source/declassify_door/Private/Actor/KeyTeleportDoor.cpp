// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/KeyTeleportDoor.h"

#include "Actor/KeyActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"

AKeyTeleportDoor::AKeyTeleportDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent*Root=CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent=Root;

	DoorMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DoorMesh->SetGenerateOverlapEvents(true);
	
	TeleportTargetLocation = FVector(0, 100, 0);
}

void AKeyTeleportDoor::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> FoundKeys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeyActor::StaticClass(), FoundKeys);
	
	for (AActor* KeyActor : FoundKeys)
	{
		if (AKeyActor* Key = Cast<AKeyActor>(KeyActor))
		{
			Key->OnKeyPickedUp.AddDynamic(this, &AKeyTeleportDoor::OnKeyPickedUp);
		}
	}
}

void AKeyTeleportDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyTeleportDoor::OnInteract_Implementation(AActor* Interactor)
{
	if(!Interactor)
	{
		return;
	}

	if(bCanTeleport)
	{
		if(ACharacter* Character = Cast<ACharacter>(Interactor))
		{
			
			FVector CharacterLocation = Character->GetActorLocation();
			FRotator CharacterRotation = Character->GetActorRotation();
			
			FVector ForwardVector = CharacterRotation.Vector();  
			FVector TeleportLocation = CharacterLocation + (ForwardVector * 500.0f); 
			
			TeleportLocation.Z += 30.0f;  
			
			if(TeleportSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, GetActorLocation());
			}
			
			Character->SetActorLocation(TeleportLocation);
		
		}
	}
	else
	{
		if(DoorLockSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorLockSound, GetActorLocation());
		}
	}
	
}

void AKeyTeleportDoor::SetTeleportTarget(const FVector& NewTargetLocation)
{
	TeleportTargetLocation = NewTargetLocation;
}

void AKeyTeleportDoor::OnKeyPickedUp()
{
	bCanTeleport = true;
}

