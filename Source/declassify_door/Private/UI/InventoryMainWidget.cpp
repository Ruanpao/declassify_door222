// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryMainWidget.h"
#include "UI/InventoryMainWidget.h"
#include "Component/InventoryComponent.h"
#include "UI/InventoryCellWidget.h"
#include "UI/InventoryCellWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMainWidget,All,All);


UInventoryMainWidget::~UInventoryMainWidget()
{
}

void UInventoryMainWidget::ReceivedInfo(int32 P_SlotIndex)
{
}

void UInventoryMainWidget::BrushWhite()
{
}

void UInventoryMainWidget::ReceivedRemoveInfo(int32 P_SlotIndex)
{
}

void UInventoryMainWidget::NativePreConstruct()
{

	
	Super::NativePreConstruct();
}

void UInventoryMainWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (GetWorld())
    {
        TArray<AActor*> PlayerTaggedActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
        
        for (AActor* Actor : PlayerTaggedActors)
        {
            if (APawn* PlayerPawn = Cast<APawn>(Actor))
            {
                if (UInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UInventoryComponent>())
                {
                    // 仅在未绑定时执行绑定
                    if (!bIsDelegateBound)
                    {
                        InventoryComponent->OnInventoryUpdate.AddUObject(this, &UInventoryMainWidget::UpdateMainWidget);
                        bIsDelegateBound = true;
                        UE_LOG(LogMainWidget, Log, TEXT("Delegate bound successfully"));
                    }
                    else
                    {
                        UE_LOG(LogMainWidget, Log, TEXT("Delegate already bound, skipping"));
                    }
                    
                    int32 Index = 0;
                    for (auto& Item : InventoryComponent->Slot)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Nakamura01 ID: %s , Quantity: %d , Index: %d, Name:%s"), 
                               *Item.ID.ToString(), Item.Quantity, Index, *GetName());
                        
                        if (UInventoryCellWidget* InventoryCellWidget = 
                            CreateWidget<UInventoryCellWidget>(GetWorld(), InventoryCellWidgetClass))
                        {
                            InventoryCellWidget->UpdateInventoryCellWidget(Item.ID, Item.Quantity, Index);
                            InventoryCellWidget->Received_1.AddUObject(this, &UInventoryMainWidget::ReceivedInfo);
                            InventoryCellWidget->BrushWhite.AddUObject(this, &UInventoryMainWidget::BrushWhite);
                            InventoryCellWidget->Remove.AddUObject(this, &UInventoryMainWidget::ReceivedRemoveInfo);
                            Grid->AddChildToWrapBox(InventoryCellWidget);
                            Index += 1;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void UInventoryMainWidget::UpdateMainWidget()
{
}
