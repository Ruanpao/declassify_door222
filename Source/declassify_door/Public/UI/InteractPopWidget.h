// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "declassify_door/declassify_doorCharacter.h" 
#include "declassify_door/CoreTypes/ItemCoreType.h"
#include "InteractPopWidget.generated.h"

/**
 * 
 */


DECLARE_MULTICAST_DELEGATE_FiveParams(FWhetherClickedAndNearTower, bool , bool , bool , bool , bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FSendHeldItem, FItemInInventory)

UCLASS()
class DECLASSIFY_DOOR_API UInteractPopWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	FSendHeldItem SendHeldItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* DataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HeldItem")
	FItemInInventory HeldItem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_TakeInHand;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UVerticalBox* VerticalBox;

protected:
	virtual void NativeOnInitialized() override;
	void UpdateHeldItem(FItemInInventory NewHeldItem);

};
