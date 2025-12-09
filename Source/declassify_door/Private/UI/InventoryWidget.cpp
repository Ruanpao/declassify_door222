// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "UI/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Components/VerticalBox.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Super::NativeConstruct();

    SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct called"));

    // 只在InventoryComponent有效但尚未初始化时进行初始化
    if (InventoryComponent && !bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct - Initializing inventory"));
        InitializeInventory(InventoryComponent);
    }
    else if (!InventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct - InventoryComponent is NULL"));
    }

    if (SlotContainer)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct - SlotContainer is valid"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::NativeConstruct - SlotContainer is NULL - check UMG binding!"));
    }
}

void UInventoryWidget::InitializeInventory(UInventoryComponent* InventoryComp)
{
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::InitializeInventory - Start"));
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::InitializeInventory - Already initialized, skipping"));
        return;
    }
    
    InventoryComponent = InventoryComp;
    
    if (!InventoryComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::InitializeInventory - InventoryComponent is NULL"));
        return;
    }
    if (!SlotWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::InitializeInventory - SlotWidgetClass is NULL"));
        return;
    }
    if (!SlotContainer)
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::InitializeInventory - SlotContainer is NULL"));
        return;
    }

    // 检查SlotWidgetClass是否有效
    UClass* WidgetClass = SlotWidgetClass.Get();
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::InitializeInventory - SlotWidgetClass is not valid"));
        return;
    }

    // 确保创建的Widget是UInventorySlotWidget类型
    if (!WidgetClass->IsChildOf(UInventorySlotWidget::StaticClass()))
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::InitializeInventory - SlotWidgetClass is not a UInventorySlotWidget subclass"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::InitializeInventory - All components valid, creating %d slots"), 10);

    // 清空现有槽位
    SlotContainer->ClearChildren();
    SlotWidgets.Empty();

    // 创建10个槽位
    for (int32 i = 0; i < 10; i++)
    {
        UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, WidgetClass);
        if (SlotWidget)
        {
            SlotWidget->SlotIndex = i;
            SlotContainer->AddChildToVerticalBox(SlotWidget);
            SlotWidgets.Add(SlotWidget);
            SlotWidget->ItemDataTable = InventoryComp->Datatable;
            
            UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::InitializeInventory - Created slot %d, Widget: %s"), 
                i, *SlotWidget->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::InitializeInventory - Failed to create slot widget %d"), i);
        }
    }

    // 绑定委托
    InventoryComponent->OnInventoryUpdate.AddUObject(this, &UInventoryWidget::OnInventoryUpdated);
    InventoryComponent->HeldChanged.AddUObject(this, &UInventoryWidget::OnHeldItemChanged);

    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::InitializeInventory - Delegates bound"));

    // 初始刷新
    RefreshInventory();
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::InitializeInventory - Completed"));
    bInitialized = true;

}

void UInventoryWidget::SelectSlot(int32 SlotIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SelectSlot - SlotIndex: %d, TotalSlots: %d"), 
        SlotIndex, SlotWidgets.Num());

    if (SlotIndex < 0 || SlotIndex >= SlotWidgets.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::SelectSlot - Invalid slot index %d"), SlotIndex);
        return;
    }

    // 取消所有槽位的选中状态
    for (UInventorySlotWidget* SlotWidget : SlotWidgets)
    {
        if (SlotWidget)
        {
            SlotWidget->SetAsSelected(false);
        }
    }

    // 选中指定槽位
    if (SlotWidgets[SlotIndex])
    {
        SlotWidgets[SlotIndex]->SetAsSelected(true);
        UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::SelectSlot - Slot %d selected successfully"), SlotIndex);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::SelectSlot - SlotWidget at index %d is NULL"), SlotIndex);
    }
}

void UInventoryWidget::OnInventoryUpdated()
{
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnInventoryUpdated - Refreshing inventory"));
    RefreshInventory();
}

void UInventoryWidget::OnHeldItemChanged(FItemInInventory HeldItem)
{
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::OnHeldItemChanged - HeldItem Index: %d, ID: %s"), 
        HeldItem.Index, *HeldItem.ID.ToString());
    SelectSlot(HeldItem.Index);
}

void UInventoryWidget::RefreshInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::RefreshInventory - Start"));
    
    if (!InventoryComponent) 
    {
        UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::RefreshInventory - InventoryComponent is null"));
        return;
    }

    const TArray<FItemInInventory>& Slots = InventoryComponent->Slot;
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::RefreshInventory - Refreshing inventory with %d slots"), Slots.Num());
    
    // 打印所有槽位信息
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::RefreshInventory - Slot %d: ID=%s, Quantity=%d"), 
            i, *Slots[i].ID.ToString(), Slots[i].Quantity);
    }
    
    for (int32 i = 0; i < SlotWidgets.Num() && i < Slots.Num(); i++)
    {
        if (SlotWidgets[i])
        {
            UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::RefreshInventory - Setting slot %d data"), i);
            SlotWidgets[i]->SetSlotData(Slots[i]);
            
            bool bIsSelected = (i == InventoryComponent->HeldItem.Index);
            SlotWidgets[i]->SetAsSelected(bIsSelected);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UInventoryWidget::RefreshInventory - SlotWidget %d is NULL"), i);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::RefreshInventory - Completed"));
}