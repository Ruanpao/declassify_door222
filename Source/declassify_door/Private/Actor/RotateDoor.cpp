// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RotateDoor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

ARotateDoor::ARotateDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

}

void ARotateDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARotateDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARotateDoor::RotateDoor()
{
	if(bIsRotated)
	{
		return;
	}

	bIsRotated = true;
	
	FRotator CurrentRotation = DoorMesh->GetRelativeRotation();
	CurrentRotation.Roll += 180.0f;
	DoorMesh->SetRelativeRotation(CurrentRotation);

	FVector DoorUp = DoorMesh->GetUpVector();
	FVector DoorDown = -DoorUp;
	
	FVector CurrentLocation = DoorMesh->GetRelativeLocation();
	CurrentLocation += DoorDown * 340.0f; 
	
	DoorMesh->SetRelativeLocation(CurrentLocation);
	
	bIsRotated = false;

	bIsUpsideDown = !bIsUpsideDown;
}

void ARotateDoor::OnInteract_Implementation(AActor* Interactor)
{
	if(!Interactor)
	{
		return;
	}

	if(ACharacter* Character = Cast<ACharacter>(Interactor))
	{
			
		FVector CharacterLocation = Character->GetActorLocation();
		FRotator CharacterRotation = Character->GetActorRotation();
			
		FVector ForwardVector = CharacterRotation.Vector();  
		FVector TeleportLocation = CharacterLocation + (ForwardVector * 500.0f); 
		
		
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			if (bIsUpsideDown)
			{
				TeleportLocation.Z -= 300.0f;  
			}
			else
			{
				TeleportLocation.Z += TeleportDistance;
				bIsRotated = !bIsRotated;
			}
		}
		
		if(TeleportSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, GetActorLocation());
		}
			
		Character->SetActorLocation(TeleportLocation);
		
	}
}

void ARotateDoor::SetTeleportTarget(const FVector& NewTargetLocation)
{
	TeleportTargetLocation = NewTargetLocation;
}

