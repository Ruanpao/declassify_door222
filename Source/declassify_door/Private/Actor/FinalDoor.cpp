// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/FinalDoor.h"
#include "Actor/DoorPiece.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenu/MainMenuPlayerController.h"

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
            PieceActor->OnDoorOpened.AddDynamic(this, &AFinalDoor::AddKey);
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
        // 启用碰撞，允许交互
        //DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        
        if(DoorCreateSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, DoorCreateSound, GetActorLocation());
        }
    }
}

void AFinalDoor::OnInteract_Implementation(AActor* Interactor)
{
    // 只有在门可见时才允许交互
    if (DoorMesh->IsVisible())
    {
        UE_LOG(LogTemp, Warning, TEXT("FinalDoor interacted - Returning to main menu"));

        if(InteractSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
        }
        
        // 回到主菜单
        ReturnToMainMenu();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FinalDoor is not visible - Interaction ignored"));
    }
}

void AFinalDoor::ReturnToMainMenu()
{
        // 备用方案：直接加载主菜单地图
    UGameplayStatics::OpenLevel(GetWorld(), "MenuMap");
}

