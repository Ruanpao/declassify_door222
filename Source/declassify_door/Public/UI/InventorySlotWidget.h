#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "declassify_door/CoreTypes/ItemCoreType.h"
#include "InventorySlotWidget.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSlotData(const FItemInInventory& ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetAsSelected(bool bSelected);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotBorder;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FLinearColor SelectedColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FLinearColor NormalColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f);
};