// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

#include "declassify_door/declassify_doorCharacter.h"
#include "UI/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventory, All, All);


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_Item.DT_Item'"));
    
	if (Datatable)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::Constructor - DataTable loaded successfully: %s"), *Datatable->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::Constructor - Failed to load DataTable"));
	}
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Start"));
    
    // 初始化库存槽位
    UpdateSlot();
    
    // 设置初始手持物品为第一个槽位
    HeldItem = Slot[5];
    UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Initial HeldItem: ID=%s, Quantity=%d, Index=%d"), 
        *HeldItem.ID.ToString(), HeldItem.Quantity, HeldItem.Index);
    
    // 绑定到玩家HUD的委托
    AActor* OwnerActor = GetOwner();
    if (OwnerActor && OwnerActor->ActorHasTag(FName("Player")))
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Owner is player, setting up HUD bindings"));
        
        if (APawn* PlayerPawn = Cast<APawn>(OwnerActor))
        {
            APlayerController* PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
            if (PlayerController)
            {
                if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
                {
                    HUD->OnHoledSlotChanged.AddDynamic(this, &UInventoryComponent::UpdateHeldSlot);
                    HUD->RemoveItem.AddDynamic(this, &UInventoryComponent::RemoveFromInventory);
                    UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - HUD delegates bound successfully"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::BeginPlay - HUD is not APlayerHUD"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::BeginPlay - PlayerController is NULL"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Owner is not player or has no player tag"));
    }
    
    // 测试：添加一个物品到库存
    UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Adding test item ID '4' to inventory"));
    AddToInventory(FName("4"), 1);
	AddToInventory(FName("3"), 1);


    // 打印所有槽位信息
    UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Current inventory slots:"));
    for(auto Item : Slot)
    {
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Slot %d: ID=%s, Quantity=%d"), 
            Item.Index, *Item.ID.ToString(), Item.Quantity);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay - Completed"));
}

bool UInventoryComponent::AddToInventory(const FName Item_ID, int32 Quantity)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddToInventory - ItemID: %s, Quantity: %d"), 
		*Item_ID.ToString(), Quantity);
    
	int32 LocalQuantityRemaining = Quantity;
	bool LocalHasFailed = false;

	while(LocalQuantityRemaining > 0 && !LocalHasFailed)
	{
		FFindSlot FindSlotResult = FindSlot(Item_ID);
        
		if(FindSlotResult.FindSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddToInventory - Found existing slot at index %d"), 
				FindSlotResult.SlotIndex);
			AddOne(FindSlotResult.SlotIndex, 1);
			OnInventoryUpdate.Broadcast();

			if(FindSlotResult.SlotIndex == HeldItem.Index)
			{
				UpdateHeldSlot(FindSlotResult.SlotIndex);
			}

			LocalQuantityRemaining -= 1;
		}
		else if (AnyEmptySlotAvailable() >= 0)
		{
			int32 EmptySlotIndex = AnyEmptySlotAvailable();
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddToInventory - Creating new slot at index %d"), 
				EmptySlotIndex);
			CreateNewSlot(Item_ID, EmptySlotIndex);
			OnInventoryUpdate.Broadcast();

			if(EmptySlotIndex == HeldItem.Index)
			{
				UpdateHeldSlot(EmptySlotIndex);
			}

			LocalQuantityRemaining -= 1;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddToInventory - No available slots"));
			LocalHasFailed = true;
		}
	}

	return LocalHasFailed;
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
			if(FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(Item_ID , "/Script/Engine.DataTable'/Game/DataTable/DT_Item.DT_Item'"))
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
	Slot[Index].Quantity += Quantity;

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
	// 从数据表中获取物品信息
	if (FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(Item_ID, TEXT("")))
	{
		Slot[Index].ID = Item_ID;
		Slot[Index].Quantity = 1;
		Slot[Index].Name = FoundItemInfo->Name.ToString(); // 从数据库读取名称
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::CreateNewSlot - Created slot at index %d with item %s (Name: %s)"), 
			Index, *Item_ID.ToString(), *Slot[Index].Name);
	}
	else
	{
		// 如果找不到物品信息，使用默认值
		Slot[Index].ID = Item_ID;
		Slot[Index].Quantity = 1;
		Slot[Index].Name = TEXT("Unknown Item");
		UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::CreateNewSlot - Failed to find item info for ID: %s"), *Item_ID.ToString());
	}
}

void UInventoryComponent::UpdateSlot()
{
	int32 SlotIndex = 0;
	
	while(Slot.Num() < SlotSize)
	{
		FItemInInventory NewSlot;
		NewSlot.ID = "0000";
		NewSlot.Quantity = 0;
		NewSlot.Index = SlotIndex;
		NewSlot.Name = "NoneItem"; // 设置默认名称
		
		Slot.Add(NewSlot);

		SlotIndex += 1;
	}
	OnInventoryUpdate.Broadcast();
}

void UInventoryComponent::RemoveFromInventory(int32 Index, bool RemoveAll, bool IsConsumed)
{
	if(Slot[Index].Quantity == 1 or RemoveAll)
	{
		if(IsConsumed)
		{
			DestroyAOldSlot(Index);
			UE_LOG(LogInventory, Error ,TEXT("I am Consumed"));

			OnInventoryUpdate.Broadcast();

			if(Index == HeldItem.Index)
			{
				UpdateHeldSlot(Index);
			}
		}
		else
		{
			DestroyAOldSlot(Index);

			OnInventoryUpdate.Broadcast();

			if(Index == HeldItem.Index)
			{
				UpdateHeldSlot(Index);

				UE_LOG(LogInventory , Warning , TEXT(" %d %d --- %s %d"),Index, HeldItem.Index , *HeldItem.ID.ToString(), HeldItem.Quantity);
			}
		}
	}
	else
	{
		if(IsConsumed)
		{
			RemoveOne(Index , 1);
			UE_LOG(LogInventory, Error ,TEXT("I am Consumed"));

			OnInventoryUpdate.Broadcast();

			if(Index == HeldItem.Index)
			{
				UpdateHeldSlot(Index);
			}
		}
		else
		{
			RemoveOne(Index, 1);

			OnInventoryUpdate.Broadcast();
			
			if(Index == HeldItem.Index)
			{
				UpdateHeldSlot(Index);

				UE_LOG(LogInventory , Warning , TEXT(" %d %d --- %s %d"),Index, HeldItem.Index , *HeldItem.ID.ToString(), HeldItem.Quantity);
			}
		}
	}
}

void UInventoryComponent::RemoveOne(int32 Index, int32 Quantity)
{
	Slot[Index].Quantity -= Quantity;

}

void UInventoryComponent::DestroyAOldSlot(int32 Index)
{
	Slot[Index].ID = "0000";
	Slot[Index].Quantity = 0;
	Slot[Index].Name = "NoneItem"; // 重置名称

}

void UInventoryComponent::UpdateHeldSlot(int32 Index)
{
	HeldItem = Slot[Index];
	
	HeldChanged.Broadcast(HeldItem);

	// 显示物品名称
	APlayerController* PlayerController = nullptr;
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	}
    
	if (PlayerController)
	{
		APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			FString ItemName = HeldItem.Name;
			if (!ItemName.IsEmpty())
			{
				HUD->ShowNewItemNameImmediately(ItemName);
			}
		}
	}

	Adeclassify_doorCharacter* MyCharacter = Cast<Adeclassify_doorCharacter>(GetOwner());
	if(MyCharacter)
	{
		if(HeldItem.ID==FName("2"))
		{
			MyCharacter->PickupPaint(FLinearColor::Red);
			MyCharacter->bHasPaint=true;
			UE_LOG(LogInventory , Warning , TEXT("RED"));

		}
		else if(HeldItem.ID==FName("3"))
		{
			MyCharacter->PickupPaint(FLinearColor::Green);
			MyCharacter->bHasPaint=true;
			UE_LOG(LogInventory , Warning , TEXT("GREEN"));

		}
		else if(HeldItem.ID==FName("4"))
		{
			MyCharacter->PickupPaint(FLinearColor::Blue);
			MyCharacter->bHasPaint=true;
			UE_LOG(LogInventory , Warning , TEXT("BLUE"));
		}
		else if(HeldItem.ID==FName("5"))
		{
			MyCharacter->PickupPaint(FLinearColor::White);
			MyCharacter->bHasPaint=true;
			UE_LOG(LogInventory , Warning , TEXT("WHITE"));
		}
		else
		{
			MyCharacter->bHasPaint=false;
			UE_LOG(LogInventory , Warning , TEXT("bHasPaint=false"));

		}
	}

	UE_LOG(LogInventory , Warning , TEXT("HeldItem Changed, ID : %s , Quantity : %d"), *HeldItem.ID.ToString(), HeldItem.Quantity);
}
