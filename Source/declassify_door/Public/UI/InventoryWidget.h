// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "UI/InventorySlotWidget.h"
#include "declassify_door/CoreTypes/ItemCoreType.h"

#include "InventoryWidget.generated.h"

/**
 * 
 */


UCLASS()
class DECLASSIFY_DOOR_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory(UInventoryComponent* InventoryComp);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectSlot(int32 SlotIndex);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SlotContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	UPROPERTY()
	TArray<UInventorySlotWidget*> SlotWidgets;

	UFUNCTION()
	void OnInventoryUpdated();

	UFUNCTION()
	void OnHeldItemChanged(FItemInInventory HeldItem);

	void RefreshInventory();
};