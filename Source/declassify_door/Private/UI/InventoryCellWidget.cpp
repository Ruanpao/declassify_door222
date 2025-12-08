// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryCellWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogCellWidget,All,All)


void UInventoryCellWidget::UpdateInventoryCellWidget(FName P_ID, int32 P_Quantity, int32 P_SlotIndex)
{
	ID = P_ID;

	if(!Datatable)
	{
		Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ItemDataTable.DT_ItemDataTable'"));
	}

	if(Datatable)
	{
		if (FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(ID , ""))
		{
			Icon = FoundItemInfo->Icon;
		}
	}
}

void UInventoryCellWidget::OnButtonClicked()
{
	BrushWhite.Broadcast();
	
	if(OuterBorder)
	{
		OuterBorder->SetBrushColor(FLinearColor::Green);
	}

	//ReceivedInfo.Broadcast(ID);
}

void UInventoryCellWidget::OnButtonPressed()
{
	if(OuterBorder)
	{
		OuterBorder->SetBrushColor(FLinearColor::Yellow);
	}
}

void UInventoryCellWidget::OnMouseButtonDown_Right()
{
}

void UInventoryCellWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	/*
	if(Button_0)
	{
		Button_0->OnClicked.AddDynamic(this , &UShopCellWidget::OnButtonClicked);

		Button_0->OnPressed.AddDynamic(this , &UShopCellWidget::OnButtonPressed);
	}
	*/
}
