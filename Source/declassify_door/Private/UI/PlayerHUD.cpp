// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PlayerHUD.h"

#include "Engine/Canvas.h"
#include "UI/InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/InteractPromptWidget.h"

#include "declassify_door/declassify_doorPlayerController.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "UI/ItemNameWidget.h"

APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	// 初始化UI
	InitializeUI();
}

void APlayerHUD::InitializeUI()
{
    // 确保我们有有效的玩家控制器
    if (!GetOwningPlayerController())
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerHUD: No owning player controller!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("=== PlayerHUD InitializeUI Start ==="));

    // 创建背包UI
    if (InventoryWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryWidgetClass is valid"));
        
        InventoryWidget = CreateWidget<UInventoryWidget>(GetOwningPlayerController(), InventoryWidgetClass);
        if (InventoryWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("InventoryWidget created successfully"));
            
            // 设置UI属性确保可见
            InventoryWidget->SetVisibility(ESlateVisibility::Visible);
            InventoryWidget->SetIsEnabled(true);
            
            // 获取玩家库存组件并初始化UI
            UInventoryComponent* InventoryComp = GetPlayerInventoryComponent();
            if (InventoryComp)
            {
                UE_LOG(LogTemp, Warning, TEXT("InventoryComponent found, initializing inventory..."));
                InventoryWidget->InitializeInventory(InventoryComp);
                
                // 直接添加到视口，保持持续显示
                InventoryWidget->AddToViewport();
                
                UE_LOG(LogTemp, Warning, TEXT("InventoryWidget added to viewport with ZOrder 100"));
                
                // 检查UI是否真的在视口中
                if (InventoryWidget->IsInViewport())
                {
                    UE_LOG(LogTemp, Warning, TEXT("InventoryWidget is confirmed to be in viewport"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("InventoryWidget is NOT in viewport!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("PlayerHUD: Failed to get player inventory component"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("PlayerHUD: Failed to create inventory widget"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerHUD: InventoryWidgetClass is not set"));
    }
	if (InteractPromptWidgetClass)
	{
		InteractPromptWidget = CreateWidget<UInteractPromptWidget>(GetOwningPlayerController(), InteractPromptWidgetClass);
		if (InteractPromptWidget)
		{
			InteractPromptWidget->AddToViewport();
			InteractPromptWidget->SetVisibility(ESlateVisibility::Collapsed); // 初始隐藏
			UE_LOG(LogTemp, Warning, TEXT("InteractPromptWidget created and added to viewport"));
		}
	}

	if (ItemNameWidgetClass)
	{
		ItemNameWidget = CreateWidget<UItemNameWidget>(GetOwningPlayerController(), ItemNameWidgetClass);
		if (ItemNameWidget)
		{
			ItemNameWidget->AddToViewport();
			ItemNameWidget->HideImmediately(); // 初始隐藏
			UE_LOG(LogTemp, Warning, TEXT("ItemNameWidget created and added to viewport"));
		}
	}
	
    UE_LOG(LogTemp, Warning, TEXT("=== PlayerHUD InitializeUI End ==="));
}

void APlayerHUD::UpdateHeldSlot(int32 SlotIndex)
{
	// 广播委托，通知库存组件更新手持物品
	OnHoledSlotChanged.Broadcast(SlotIndex);
	UE_LOG(LogTemp, Log, TEXT("PlayerHUD: Updated held slot to index %d"), SlotIndex);
}

void APlayerHUD::RemoveItemFromInventory(int32 SlotIndex, bool RemoveAll, bool IsConsumed)
{
	// 广播委托，通知库存组件移除物品
	RemoveItem.Broadcast(SlotIndex, RemoveAll, IsConsumed);
	UE_LOG(LogTemp, Log, TEXT("PlayerHUD: Requested removal of item from slot %d (RemoveAll: %s, Consumed: %s)"), 
		SlotIndex, RemoveAll ? TEXT("True") : TEXT("False"), IsConsumed ? TEXT("True") : TEXT("False"));
}

UInventoryComponent* APlayerHUD::GetPlayerInventoryComponent() const
{
	APawn* PlayerPawn = GetOwningPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD: No owning pawn"));
		return nullptr;
	}

	// 从玩家Pawn获取InventoryComponent
	UInventoryComponent* InventoryComp = PlayerPawn->FindComponentByClass<UInventoryComponent>();
	if (!InventoryComp)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD: No InventoryComponent found on player pawn"));
		return nullptr;
	}

	return InventoryComp;
}

void APlayerHUD::ShowInteractPrompt(const FText& PromptText)
{
	if (InteractPromptWidget)
	{
		InteractPromptWidget->SetInteractText(PromptText);
		InteractPromptWidget->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Showing interact prompt: %s"), *PromptText.ToString());
	}
}

void APlayerHUD::HideInteractPrompt()
{
	if (InteractPromptWidget)
	{
		InteractPromptWidget->SetVisibility(ESlateVisibility::Collapsed);
		UE_LOG(LogTemp, Warning, TEXT("Hiding interact prompt"));
	}
}

void APlayerHUD::ShowItemName(const FString& ItemName)
{
	if (!ItemNameWidget) return;
    
	// 如果正在显示物品名称，先停止之前的定时器
	if (IsItemNameShowing())
	{
		GetWorldTimerManager().ClearTimer(ItemNameTimerHandle);
	}
    
	// 设置物品名称并显示
	ItemNameWidget->SetItemName(ItemName);
	ItemNameWidget->ShowItemName();
    
	// 设置定时器，1秒后隐藏
	GetWorldTimerManager().SetTimer(ItemNameTimerHandle, this, &APlayerHUD::HideItemName, 1.0f, false);
    
	UE_LOG(LogTemp, Warning, TEXT("Showing item name: %s"), *ItemName);
}

// 添加缺失的函数实现
void APlayerHUD::ShowNewItemNameImmediately(const FString& ItemName)
{
	if (!ItemNameWidget) return;
    
	// 清除之前的定时器
	GetWorldTimerManager().ClearTimer(ItemNameTimerHandle);
    
	// 立即隐藏当前显示
	ItemNameWidget->HideImmediately();
    
	// 显示新物品名称
	ShowItemName(ItemName);
}

// 添加缺失的函数实现
bool APlayerHUD::IsItemNameShowing() const
{
	return ItemNameWidget && ItemNameWidget->IsVisible();
}

void APlayerHUD::HideItemName()
{
	if (ItemNameWidget)
	{
		ItemNameWidget->HideItemName();
		UE_LOG(LogTemp, Warning, TEXT("Hiding item name"));
	}
}