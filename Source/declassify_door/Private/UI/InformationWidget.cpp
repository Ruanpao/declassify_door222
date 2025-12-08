// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InformationWidget.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogInformationWidget, All, All)


void UInformationWidget::ShowInfo(FItemInInventory HoldedItem)
{
	ID = HoldedItem.ID;
	Quantity = HoldedItem.Quantity;

	UE_LOG(LogTemp,Warning,TEXT("HHEELLOO"));

	if(!Datatable)
	{
		Datatable = LoadObject<UDataTable>(this, TEXT(""));
	}

	if(Datatable)
	{
		if(ID == "0000")
		{
			Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));
			
			if(DetailBox && DescriptionBox && InformationBox)
			{
				DetailBox->SetVisibility(ESlateVisibility::Hidden);
				DescriptionBox->SetVisibility(ESlateVisibility::Hidden);
				InformationBox->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(ID , "");

			if(FoundItemInfo)
			{
				Icon = FoundItemInfo->Icon;
				Name = FText::FromName(FoundItemInfo->Name);
			}

			if(DetailBox && DescriptionBox && InformationBox)
			{
				DetailBox->SetVisibility(ESlateVisibility::Visible);
				DescriptionBox->SetVisibility(ESlateVisibility::Visible);
				InformationBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UInformationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	if(GetWorld())
	{
		TArray<AActor*> PlayerTaggedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
    
		// 遍历查找符合条件的Pawn
		for (AActor* Actor : PlayerTaggedActors)
		{
			if (APawn* PlayerPawn = Cast<APawn>(Actor))
			{
				if (UInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UInventoryComponent>())
				{
					InventoryComponent->HeldChanged.AddUObject(this, &UInformationWidget::ShowInfo);
					break;
				}
			}
		}
	}
}
