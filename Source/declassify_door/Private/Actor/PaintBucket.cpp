// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PaintBucket.h"
#include "Components/StaticMeshComponent.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "GameFramework/Character.h"

APaintBucket::APaintBucket()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BucketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BucketMesh"));
	BucketMesh->SetupAttachment(RootComponent);
}

void APaintBucket::BeginPlay()
{
	Super::BeginPlay();
	
}

void APaintBucket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APaintBucket::OnInteract_Implementation(AActor* Interactor)
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
			if(MyCharacter)
			{
				MyCharacter->PickupPaint(PaintColor);
			}
		}
		Destroy();
	}
}

