// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "declassify_door/CoreTypes/ItemCoreType.h"
#include "Component/InventoryComponent.h"
#include "Components/VerticalBox.h"
#include "InformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class DECLASSIFY_DOOR_API UInformationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* Datatable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	FText Name = FText::FromString("None");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	int32 Quantity = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	FText Description = FText::FromString("Invalid Description");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	FText Information = FText::FromString("Invalid Information");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	FText ItemType = FText::FromString("Invalid ItemType");


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UVerticalBox* DetailBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UVerticalBox* DescriptionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UVerticalBox* InformationBox;
	
	void ShowInfo(FItemInInventory HoldedItem);

protected:
	void NativeOnInitialized() override;
};
