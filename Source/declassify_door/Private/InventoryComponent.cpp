// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventory, All, All);


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Datatable = LoadObject<UDataTable>(this, TEXT("")); //配置我的数据表路径

	
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateSlot();
	HoldedItem = Slot[0];
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		UE_LOG(LogInventory, Error, TEXT("Inventory has no owner actor!"));
		return;
	}
/*
	if (OwnerActor->ActorHasTag(FName("Player")))
	{
		// 确保 Owner 是 Pawn（玩家角色）
		if (APawn* PlayerPawn = Cast<APawn>(OwnerActor))
		{
			// 获取玩家控制器
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController && PlayerController->GetPawn() == PlayerPawn)
			{
				// 获取 HUD 并绑定委托（仅玩家 Inventory 执行）
				if (APVZ3DPlayerHUD* HUD = Cast<APVZ3DPlayerHUD>(PlayerController->GetHUD()))
				{
					HUD->Buy.AddUObject(this, &UPVZ3DInventoryComponent::Buy);
					HUD->OnHoledSlotChanged.AddUObject(this, &UPVZ3DInventoryComponent::UpdateHoldedSlot);
					HUD->RemoveItem.AddUObject(this, &UPVZ3DInventoryComponent::RemoveFromInventory);
                    
					UE_LOG(LogInventory, Log, TEXT("Player Inventory bound to HUD successfully"));
				}
			}
		}
	}
	*/
	else
	{
		UE_LOG(LogInventory, Log, TEXT("This is not a player inventory, skipping HUD binding"));
	}
	
}

bool UInventoryComponent::AddToInventory(const FName Item_ID, int32 Quantity)
{
	return true;
}

FFindSlot UInventoryComponent::FindSlot(FName Item_ID)
{
	FFindSlot ReturnResult;
	ReturnResult.FindSlot = false;
	ReturnResult.SlotIndex = -1;
	
	for (int32 index = 0 ; index < SlotSize ; index++)
	{
		if(Item_ID == Slot[index].ID)
		{
			if(FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(Item_ID , ""))
			{
				if(Slot[index].Quantity < FoundItemInfo->MaxStackNum)
				{
					ReturnResult.FindSlot = true;
					ReturnResult.SlotIndex = index;
					return ReturnResult;
				}
			}
			else
			{
				return ReturnResult;
			}
		}
	}
	return ReturnResult;
}

void UInventoryComponent::AddOne(int32 Index, int32 Quantity)
{
}

int32 UInventoryComponent::AnyEmptySlotAvailable() const
{
	for (int32 index = 0 ; index < SlotSize ; index++)
	{
		if (Slot[index].Quantity == 0 || Slot[index].ID == FName("0000"))
		{
			return index;
		}
	}

	return -1;
}

void UInventoryComponent::CreateNewSlot(FName Item_ID, int32 Index)
{
}

void UInventoryComponent::UpdateSlot()
{
}

void UInventoryComponent::RemoveFromInventory(int32 Index, bool RemoveAll, bool IsConsumed)
{
}

void UInventoryComponent::RemoveOne(int32 Index, int32 Quantity)
{
}

void UInventoryComponent::DestroyAOldSlot(int32 Index)
{
}

void UInventoryComponent::UpdateHoldedSlot(int32 Index)
{
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

