// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/SlotActor.h"
#include "Components/TextRenderComponent.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

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
    
    // 初始化变量
    bHasPlate = false;
    CurrentPlateColor = FLinearColor::White;
}

void ASlotActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 创建动态材质实例
    if (SlotMesh && SlotMesh->GetMaterial(0))
    {
        DynamicMaterial = SlotMesh->CreateAndSetMaterialInstanceDynamic(0);
        if (DynamicMaterial)
        {
            // 设置默认颜色
            DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), DefaultColor);
            UE_LOG(LogTemp, Warning, TEXT("SlotActor: Dynamic material created successfully with default color"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SlotActor: Failed to create dynamic material"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SlotActor: SlotMesh or base material is null"));
    }
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
    UE_LOG(LogTemp, Warning, TEXT("Has plate: %s"), bHasPlate ? TEXT("YES") : TEXT("NO"));

    if (!bHasPlate)
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

            // 设置石板颜色
            FLinearColor PlateColor = FLinearColor::White; // 默认白色
            
            if (HeldItemID == FName("7"))
            {
                PlateColor = FLinearColor::Red;
                UE_LOG(LogTemp, Warning, TEXT("Plate color set to RED"));
            }
            else if (HeldItemID == FName("8"))
            {
                PlateColor = FLinearColor::Green;
                UE_LOG(LogTemp, Warning, TEXT("Plate color set to GREEN"));
            }
            else if (HeldItemID == FName("9"))
            {
                PlateColor = FLinearColor::Blue;
                UE_LOG(LogTemp, Warning, TEXT("Plate color set to BLUE"));
            }
            else if (HeldItemID == FName("10"))
            {
                PlateColor = FLinearColor::White;
                UE_LOG(LogTemp, Warning, TEXT("Plate color set to WHITE"));
            }
            
            // 保存当前石板颜色
            CurrentPlateColor = PlateColor;
            
            // 更新SlotMesh颜色
            if (DynamicMaterial)
            {
                DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), PlateColor);
                UE_LOG(LogTemp, Warning, TEXT("SlotMesh color updated to match plate color"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No dynamic material available for color update"));
            }
            
            // 标记为有石板
            bHasPlate = true;
            
            // 触发事件
            OnPlateNumber.Broadcast(AnsNumber);
            OnPlatePlaced.Broadcast(PlateColor);
            
            UE_LOG(LogTemp, Warning, TEXT("Broadcast OnPlatePlaced with color and OnPlateNumber with value: %d"), AnsNumber);

            if (PutSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, PutSound, GetActorLocation());
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
            OnPlateRemoved.Broadcast(CurrentPlateColor);
            OnPlateNumber.Broadcast(-AnsNumber);
            
            // 重置SlotMesh颜色为默认颜色
            if (DynamicMaterial)
            {
                DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), DefaultColor);
                UE_LOG(LogTemp, Warning, TEXT("SlotMesh color reset to default"));
            }
            
            UE_LOG(LogTemp, Warning, TEXT("Broadcast OnPlateRemoved with color"));
            
            // 重置状态
            bHasPlate = false;
            StoredPlateID = FName("0");
            CurrentPlateColor = FLinearColor::White;
            
            UE_LOG(LogTemp, Warning, TEXT("Plate removed, state reset"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player hand is not empty, cannot pick up plate"));
            UE_LOG(LogTemp, Warning, TEXT("Held item ID: %s"), *Player->InventoryComponent->HeldItem.ID.ToString());
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("=== ASlotActor::OnInteract_Implementation - END ===\n"));
}