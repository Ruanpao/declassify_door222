// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemNameWidget.h"
#include "Components/TextBlock.h"


void UItemNameWidget::SetItemName(const FString& ItemName)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(FText::FromString(ItemName));
	}
}

void UItemNameWidget::ShowItemName()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UItemNameWidget::HideItemName()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UItemNameWidget::HideImmediately()
{
	SetVisibility(ESlateVisibility::Collapsed);
}