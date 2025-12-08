// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "InventoryCellWidget.h"
#include "declassify_door/CoreTypes/ItemCoreType.h"
#include "InventoryMainWidget.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FReceived_2 ,int32)

DECLARE_MULTICAST_DELEGATE_OneParam(FReceivedRemove ,  int32)

class UPVZ3DInventoryComponent;
class UPVZ3DInventoryCellWidget;


UCLASS()
class DECLASSIFY_DOOR_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	~UInventoryMainWidget();

	FReceived_2 Received_2;

	FReceivedRemove ReceivedRemove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI")
	TSubclassOf<UInventoryCellWidget> InventoryCellWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UWrapBox* Grid;
	
	UFUNCTION(BlueprintCallable)
	void ReceivedInfo(int32 P_SlotIndex);

	UFUNCTION(BlueprintCallable)
	void BrushWhite();

	UFUNCTION(BlueprintCallable)
	void ReceivedRemoveInfo(int32 P_SlotIndex);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;


	void UpdateMainWidget();

private:
	UPVZ3DInventoryComponent* PlayerInventoryComponent = nullptr;
	TArray<UPVZ3DInventoryCellWidget*> ExistingCellWidgets; 
	bool bIsDelegateBound = false; 
	
};
