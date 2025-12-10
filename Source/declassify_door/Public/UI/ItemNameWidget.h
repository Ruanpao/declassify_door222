// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ItemNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class DECLASSIFY_DOOR_API UItemNameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 设置物品名称
	UFUNCTION(BlueprintCallable, Category = "Item Name")
	void SetItemName(const FString& ItemName);

	// 显示物品名称
	UFUNCTION(BlueprintCallable, Category = "Item Name")
	void ShowItemName();

	// 隐藏物品名称
	UFUNCTION(BlueprintCallable, Category = "Item Name")
	void HideItemName();

	// 立即隐藏
	UFUNCTION(BlueprintCallable, Category = "Item Name")
	void HideImmediately();

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameText;

	// 显示持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Name")
	float DisplayDuration = 3.0f;
};
