// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Interface/ShowInfoInterface.h"
#include "PlayerHUD.generated.h"
// 前向声明
class UInventoryWidget;
class UInventoryComponent;
class UInteractPromptWidget;
class UItemNameWidget;
// 委托声明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoledSlotChanged, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRemoveItem, int32, SlotIndex, bool, RemoveAll, bool, IsConsumed);


UCLASS()
class DECLASSIFY_DOOR_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

protected:
	virtual void BeginPlay() override;

public:
	// 委托
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnHoledSlotChanged OnHoledSlotChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnRemoveItem RemoveItem;

	// 更新手持物品槽位
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateHeldSlot(int32 SlotIndex);

	// 从库存移除物品
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(int32 SlotIndex, bool RemoveAll, bool IsConsumed);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowInteractPrompt(const FText& PromptText);

	// 隐藏交互提示
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HideInteractPrompt();
	
	// 显示物品名称
	UFUNCTION(BlueprintCallable, Category = "Item Name")
	void ShowItemName(const FString& ItemName);
    
	// 立即显示新物品名称（取消之前的显示）
	UFUNCTION(BlueprintCallable, Category = "Item Name")
	void ShowNewItemNameImmediately(const FString& ItemName);

protected:
	// 背包UI类引用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	// 背包UI实例
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UInventoryWidget* InventoryWidget;

	// 初始化UI
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeUI();

	// 获取玩家库存组件
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetPlayerInventoryComponent() const;

	// 交互提示UI类引用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInteractPromptWidget> InteractPromptWidgetClass;

	// 交互提示UI实例
	UPROPERTY()
	UInteractPromptWidget* InteractPromptWidget;

	// 物品名称UI类引用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UItemNameWidget> ItemNameWidgetClass;

	// 物品名称UI实例
	UPROPERTY()
	UItemNameWidget* ItemNameWidget;

	// 定时器用于隐藏物品名称
	FTimerHandle ItemNameTimerHandle;

	// 隐藏物品名称
	UFUNCTION()
	void HideItemName();

	// 检查是否正在显示物品名称
	bool IsItemNameShowing() const;
	
};