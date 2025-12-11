// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FinalDoor.h"


AFinalDoor::AFinalDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	DoorMesh->SetVisibility(false);
}

void AFinalDoor::BeginPlay()
{
	Super::BeginPlay();

	
}

void AFinalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

