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
};