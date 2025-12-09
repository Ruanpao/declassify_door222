// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LeverActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Moving.h"

ALeverActor::ALeverActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	LeverMesh->SetupAttachment(RootComponent);

	LeverMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeverMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeverMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}


void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverActor::OnInteract_Implementation(AActor* Interactor)
{
	if(!Interactor)
	{
		return;
	}

	bIsPulled = !bIsPulled;

	FRotator NewRotation = LeverMesh->GetRelativeRotation();
	if(bIsPulled)
	{
		NewRotation.Pitch = -PullRotationAngle;

		if(PullSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,PullSound,GetActorLocation());
		}

		if(LinkedMovingActor)
		{
			LinkedMovingActor->StartMoving();
			LinkedMovingActor->StartRotating();
		}
	}
	else
	{
		NewRotation.Pitch = 0.0f;

		if(PullSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this,PullSound,GetActorLocation());
		}

		if(LinkedMovingActor)
		{
			LinkedMovingActor->StopMoving();
			LinkedMovingActor->StopRotating();
		}
	}
	LeverMesh->SetRelativeRotation(NewRotation);
}

void ALeverActor::SetLinkedMovingActor(class AMoving* MovingActor)
{
	LinkedMovingActor = MovingActor;
}

