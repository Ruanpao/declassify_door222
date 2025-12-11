// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FinalDoor.h"

#include "Actor/DoorPiece.h"
#include "Kismet/GameplayStatics.h"


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

	TArray<AActor*> FoundPieces;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoorPiece::StaticClass(), FoundPieces);

	for(AActor* Piece : FoundPieces)
	{
		ADoorPiece* PieceActor = Cast<ADoorPiece>(Piece);
		if(PieceActor)
		{
			PieceActor->OnDoorOpened.AddDynamic(this,&AFinalDoor::AddKey);
		}
	}
}

void AFinalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinalDoor::AddKey()
{
	CurrentNumberOfKeys++;
	
	if(CurrentNumberOfKeys == 3)
	{
		DoorMesh->SetVisibility(true);
	}
}

