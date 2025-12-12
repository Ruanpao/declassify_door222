// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RotateDoor.h"

#include "Actor/HidePass.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

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
	CurrentLocation += DoorDown * 260.0f; 
	
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
		FVector TeleportLocation = CharacterLocation + (ForwardVector * 350.0f); 
		
		
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			if (bIsUpsideDown)
			{
				TeleportLocation.Z -= TeleportDown;  
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

void ARotateDoor::SetDoorColor(const FLinearColor& NewColor)
{
	if(!DoorMesh) return;

	int32 MaterialIndex = 1;
	
	// 创建动态材质实例
	UMaterialInstanceDynamic* DynamicMaterial = DoorMesh->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
	if(DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("BaseColorFactor"), NewColor);
		if(PaintSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PaintSound, GetActorLocation());
		}
	}
	
	if (LinkedHidePass)
	{
		LinkedHidePass->CheckAndReveal(NewColor);
	}
}

void ARotateDoor::SetLinkedHidePass(class AHidePass* NewHidePass)
{
	LinkedHidePass = NewHidePass;

	if(LinkedHidePass)
	{
		LinkedHidePass->SetRequiredColor(RequiredPaintColor);
	}
}

