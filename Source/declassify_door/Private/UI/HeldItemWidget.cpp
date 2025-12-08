// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeldItemWidget.h"
#include "Component/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void UHeldItemWidget::UpdateHeldItemWidget(FItemInInventory HeldItem)
{
	ID = HeldItem.ID;
	Quantity = HeldItem.Quantity;
	
	if(!DataTable)
	{
		DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_Item.DT_Item'"));
	}
	
	
}

void UHeldItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(GetWorld())
	{
		TArray<AActor*> PlayerTaggedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
		for (AActor* Actor : PlayerTaggedActors)
		{
			if (APawn* PlayerPawn = Cast<APawn>(Actor))
			{
				if (UInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UInventoryComponent>())
				{
					InventoryComponent->HeldChanged.AddUObject(this, &UHeldItemWidget::UpdateHeldItemWidget);
				}
			}
		}
	}

	if(BulletNumBox && ItemQuantityBox)
	{
		BulletNumBox->SetVisibility(ESlateVisibility::Hidden);
		ItemQuantityBox->SetVisibility(ESlateVisibility::Hidden);
	}
}
