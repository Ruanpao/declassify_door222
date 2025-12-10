// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Engine/DataTable.h"

void UInventorySlotWidget::SetSlotData(const FItemInInventory& ItemData)
{
	if (!ItemDataTable) return;

	// 如果是空槽位 (ID为"0")
	if (ItemData.ID == FName("0"))
	{
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
			
		}
		return;
	}

	// 查找物品数据
	FItemBasicInfo* ItemInfo = ItemDataTable->FindRow<FItemBasicInfo>(ItemData.ID, TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_Item.DT_Item'"));
	if (ItemInfo && ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(ItemInfo->Icon);
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventorySlotWidget::SetAsSelected(bool bSelected)
{
	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(bSelected ? SelectedColor : NormalColor);
	}
}

