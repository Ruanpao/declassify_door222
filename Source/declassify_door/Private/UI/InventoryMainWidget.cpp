// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryMainWidget.h"
#include "UI/InventoryMainWidget.h"
#include "Component/InventoryComponent.h"
#include "UI/InventoryCellWidget.h"
#include "UI/InventoryCellWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMainWidget,All,All);


UInventoryMainWidget::~UInventoryMainWidget()
{
    if (bIsDelegateBound)
    {
        UE_LOG(LogMainWidget, Log, TEXT("Destroying widget, unbinding delegate"));
        bIsDelegateBound = false;
    }
}

void UInventoryMainWidget::ReceivedInfo(int32 P_SlotIndex)
{
    Received_2.Broadcast(P_SlotIndex);

}

void UInventoryMainWidget::BrushWhite()
{
    for (auto& Child : Grid->GetAllChildren())
    {
        if (UInventoryCellWidget* InventoryCellWidget = Cast<UInventoryCellWidget>(Child))
        {
            if (InventoryCellWidget->OuterBorder)
            {
                InventoryCellWidget->OuterBorder->SetBrushColor(FLinearColor::White);
            }
        }
    }
}

void UInventoryMainWidget::ReceivedRemoveInfo(int32 P_SlotIndex)
{
    ReceivedRemove.Broadcast(P_SlotIndex);
    UE_LOG(LogMainWidget, Warning, TEXT("OnMouseButtonDown_Right SlotIndex: %d"), P_SlotIndex);
}

void UInventoryMainWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

    if (GetWorld())
    {
        TArray<AActor*> PlayerTaggedActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
        
        for (AActor* Actor : PlayerTaggedActors)
        {
            if (APawn* PlayerPawn = Cast<APawn>(Actor))
            {
                if (UInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UInventoryComponent>())
                {
                    // 创建所有10个格子，包括空的
                    for (int32 i = 0; i < 10; i++) // 假设有10个槽位
                    {
                        if (UInventoryCellWidget* InventoryCellWidget = 
                            CreateWidget<UInventoryCellWidget>(GetWorld(), InventoryCellWidgetClass))
                        {
                            // 获取该槽位的物品数据
                            FItemInInventory ItemData;
                            if (i < InventoryComponent->Slot.Num())
                            {
                                ItemData = InventoryComponent->Slot[i];
                            }
                            else
                            {
                                ItemData.ID = FName("0000");
                                ItemData.Quantity = 0;
                            }
                            
                            InventoryCellWidget->UpdateInventoryCellWidget(ItemData.ID, ItemData.Quantity, i);
                            InventoryCellWidget->Received_1.AddUObject(this, &UInventoryMainWidget::ReceivedInfo);
                            InventoryCellWidget->BrushWhite.AddUObject(this, &UInventoryMainWidget::BrushWhite);
                            InventoryCellWidget->Remove.AddUObject(this, &UInventoryMainWidget::ReceivedRemoveInfo);
                            Grid->AddChildToWrapBox(InventoryCellWidget);
                        }
                    }
                    break;
                }
            }
        }
    }
}

void UInventoryMainWidget::NativeDestruct()
{
	Super::NativeDestruct();
    
}

void UInventoryMainWidget::UpdateMainWidget()
{
    UE_LOG(LogMainWidget, Log, TEXT("Updating main widget"));
    Grid->ClearChildren();

    
    NativePreConstruct(); // 保留原有逻辑，但委托不会重复绑定
}
