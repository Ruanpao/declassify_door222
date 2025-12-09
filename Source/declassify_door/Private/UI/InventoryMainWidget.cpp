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
                    // 仅在未绑定时执行绑定
                    if (!bIsDelegateBound)
                    {
                        InventoryComponent->OnInventoryUpdate.AddUObject(this, &UInventoryMainWidget::UpdateMainWidget);
                        bIsDelegateBound = true;
                        UE_LOG(LogMainWidget, Log, TEXT("Delegate bound successfully"));
                    }
                    else
                    {
                        UE_LOG(LogMainWidget, Log, TEXT("Delegate already bound, skipping"));
                    }
                    
                    int32 Index = 0;
                    for (auto& Item : InventoryComponent->Slot)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Nakamura01 ID: %s , Quantity: %d , Index: %d, Name:%s"), 
                               *Item.ID.ToString(), Item.Quantity, Index, *GetName());
                        
                        if (UInventoryCellWidget* InventoryCellWidget = 
                            CreateWidget<UInventoryCellWidget>(GetWorld(), InventoryCellWidgetClass))
                        {
                            InventoryCellWidget->UpdateInventoryCellWidget(Item.ID, Item.Quantity, Index);
                            InventoryCellWidget->Received_1.AddUObject(this, &UInventoryMainWidget::ReceivedInfo);
                            InventoryCellWidget->BrushWhite.AddUObject(this, &UInventoryMainWidget::BrushWhite);
                            InventoryCellWidget->Remove.AddUObject(this, &UInventoryMainWidget::ReceivedRemoveInfo);
                            Grid->AddChildToWrapBox(InventoryCellWidget);
                            Index += 1;
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
