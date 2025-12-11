// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/SlotActor.h"
#include "Components/TextRenderComponent.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "Kismet/GameplayStatics.h"

ASlotActor::ASlotActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    RootComponent = RootComp;

    SlotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlotMesh"));
    SlotMesh->SetupAttachment(RootComponent);

    SlotText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SlotText"));
    SlotText->SetupAttachment(RootComponent);
    SlotText->SetHorizontalAlignment(EHTA_Center);
    SlotText->SetVerticalAlignment(EVRTA_TextCenter);
    SlotText->SetText(FText::FromString(TEXT("12")));
}

void ASlotActor::BeginPlay()
{
    Super::BeginPlay();
}

void ASlotActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void ASlotActor::OnInteract_Implementation(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("=== ASlotActor::OnInteract_Implementation - START ==="));
    
    Adeclassify_doorCharacter* Player = Cast<Adeclassify_doorCharacter>(Interactor);
    if (!Player || !Player->InventoryComponent) 
    {
        UE_LOG(LogTemp, Error, TEXT("Player or InventoryComponent is null!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Current StoredPlateID: %s"), *StoredPlateID.ToString());
    UE_LOG(LogTemp, Warning, TEXT("CurrentPlate exists: %s"), CurrentPlate ? TEXT("YES") : TEXT("NO"));

    if (!CurrentPlate)
    {
        UE_LOG(LogTemp, Warning, TEXT("--- PLACEMENT MODE ---"));
        
        FName HeldItemID = Player->InventoryComponent->HeldItem.ID;
        int32 HeldItemIndex = Player->InventoryComponent->HeldItem.Index;
        
        UE_LOG(LogTemp, Warning, TEXT("Player HeldItem - Index: %d, ID: %s, Quantity: %d"), 
            HeldItemIndex, *HeldItemID.ToString(), Player->InventoryComponent->HeldItem.Quantity);

        // 检查是否是石板物品ID (7-10)
        if (HeldItemID == FName("7") || HeldItemID == FName("8") || 
            HeldItemID == FName("9") || HeldItemID == FName("10"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Valid stone plate detected: %s"), *HeldItemID.ToString());
            
            // 保存石板ID
            StoredPlateID = HeldItemID;
            UE_LOG(LogTemp, Warning, TEXT("StoredPlateID set to: %s"), *StoredPlateID.ToString());

            // 从库存移除
            Player->InventoryComponent->RemoveFromInventory(HeldItemIndex, false, false);
            UE_LOG(LogTemp, Warning, TEXT("Removed from inventory: %s"), *HeldItemID.ToString());

            // 生成石板actor
            FVector SpawnLocation = GetActorLocation() + PlateOffset;
            FRotator SpawnRotation = GetActorRotation();
            
            UE_LOG(LogTemp, Warning, TEXT("Spawning plate at: X=%.1f Y=%.1f Z=%.1f"), 
                SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
            
            // 使用FActorSpawnParameters确保正确生成
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
            AStonePlate* NewPlate = GetWorld()->SpawnActor<AStonePlate>(AStonePlate::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
            if (NewPlate)
            {
                NewPlate->ItemID = HeldItemID;
                NewPlate->bIsInSlot = true;
                
                // 确保网格组件被正确设置
                if (NewPlate->PlateMesh)
                {
                    // 设置石板网格为可见
                    NewPlate->PlateMesh->SetVisibility(true);
                    NewPlate->PlateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                    
                    UE_LOG(LogTemp, Warning, TEXT("Plate mesh component is valid and set to visible"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Plate mesh component is NULL!"));
                }
                
                // 设置石板颜色
                if (HeldItemID == FName("7"))
                {
                    NewPlate->PlateColor = FLinearColor::Red;
                    UE_LOG(LogTemp, Warning, TEXT("Plate color set to RED"));
                }
                else if (HeldItemID == FName("8"))
                {
                    NewPlate->PlateColor = FLinearColor::Green;
                    UE_LOG(LogTemp, Warning, TEXT("Plate color set to GREEN"));
                }
                else if (HeldItemID == FName("9"))
                {
                    NewPlate->PlateColor = FLinearColor::Blue;
                    UE_LOG(LogTemp, Warning, TEXT("Plate color set to BLUE"));
                }
                else if (HeldItemID == FName("10"))
                {
                    NewPlate->PlateColor = FLinearColor::White;
                    UE_LOG(LogTemp, Warning, TEXT("Plate color set to WHITE"));
                }
                
                UMaterialInstanceDynamic* DynamicMaterial = SlotMesh->CreateAndSetMaterialInstanceDynamic(0);
                if(DynamicMaterial)
                {
                    DynamicMaterial->SetVectorParameterValue(TEXT("BaseColorFactor"), CurrentPlate->PlateColor);
                }
                CurrentPlate = NewPlate;
                
                UE_LOG(LogTemp, Warning, TEXT("CurrentPlate set with ID: %s"), *CurrentPlate->ItemID.ToString());
                
                // 触发事件
                OnPlateNumber.Broadcast(AnsNumber);
                OnPlatePlaced.Broadcast(NewPlate->PlateColor);
                
                
                UE_LOG(LogTemp, Warning, TEXT("Broadcast OnPlatePlaced with color and OnPlateNumber with value: %d"), AnsNumber);

                if (PutSound)
                {
                    UGameplayStatics::PlaySoundAtLocation(this, PutSound, GetActorLocation());
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn new plate!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Held item is not a stone plate: %s"), *HeldItemID.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("--- REMOVAL MODE ---"));
        
        // 检查玩家是否空手
        if (Player->InventoryComponent->HeldItem.ID == FName("0"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Player hand is empty, can pick up plate"));
            UE_LOG(LogTemp, Warning, TEXT("Adding to inventory: %s"), *StoredPlateID.ToString());
            
            // 使用保存的ID添加到库存
            Player->InventoryComponent->AddToInventory(StoredPlateID, 1);
            
            // 触发移除事件
            OnPlateRemoved.Broadcast(CurrentPlate->PlateColor);
            OnPlateNumber.Broadcast(-AnsNumber);
            
            UE_LOG(LogTemp, Warning, TEXT("Broadcast OnPlateRemoved with color"));
            UE_LOG(LogTemp, Warning, TEXT("Destroying CurrentPlate with ID: %s"), *CurrentPlate->ItemID.ToString());
            
            // 销毁石板actor
            CurrentPlate->Destroy();
            CurrentPlate = nullptr;
            StoredPlateID = FName("0");
            
            UE_LOG(LogTemp, Warning, TEXT("CurrentPlate set to nullptr, StoredPlateID reset to: %s"), *StoredPlateID.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player hand is not empty, cannot pick up plate"));
            UE_LOG(LogTemp, Warning, TEXT("Held item ID: %s"), *Player->InventoryComponent->HeldItem.ID.ToString());
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("=== ASlotActor::OnInteract_Implementation - END ===\n"));
}