// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "declassify_door\CoreTypes\ItemCoreType.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate)

DECLARE_MULTICAST_DELEGATE_OneParam(FHeldChanged , FItemInInventory)

USTRUCT()
struct FFindSlot
{
	GENERATED_BODY()

	bool FindSlot = false;

	int32 SlotIndex = -1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DECLASSIFY_DOOR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* Datatable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InventoryComponent")
	TArray<FItemInInventory> Slot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InventoryComponent")
	int32 SlotSize = 10;

	FOnInventoryUpdate OnInventoryUpdate;

	FHeldChanged HeldChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HeldItem")
	FItemInInventory HeldItem;

	bool AddToInventory(const FName Item_ID, int32 Quantity);

	FFindSlot FindSlot(FName Item_ID);

	void AddOne(int32 Index, int32 Quantity);

	int32 AnyEmptySlotAvailable() const;

	void CreateNewSlot(FName Item_ID, int32 Index);

	void UpdateSlot();

	void RemoveFromInventory(int32 Index , bool RemoveAll , bool IsConsumed);

	void RemoveOne(int32 Index, int32 Quantity);

	void DestroyAOldSlot(int32 Index);
	
	void UpdateHeldSlot(int32 Index);

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
