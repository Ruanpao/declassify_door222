// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DoorPiece.h"

#include "Kismet/GameplayStatics.h"


ADoorPiece::ADoorPiece()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	PieceMesh->SetupAttachment(RootComponent);
}

void ADoorPiece::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
}

void ADoorPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

	float FloatingHeight = FMath::Sin(GetWorld()->GetTimeSeconds() * FloatingSpeed) * FloatngRange;
	SetActorLocation(InitialLocation + FVector(0.f, 0.f, FloatingHeight));
}

void ADoorPiece::OnInteract_Implementation(AActor* Interactor)
{
	OnDoorOpened.Broadcast();
	if(PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
	Destroy();
}

