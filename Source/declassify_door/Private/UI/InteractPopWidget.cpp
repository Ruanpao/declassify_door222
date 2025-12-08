// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractPopWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UInteractPopWidget::OnButton_TakeInHandClicked()
{
}

void UInteractPopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SendHeldItem.AddUObject(this, &UInteractPopWidget::UpdateHeldItem);

	if(!DataTable)
	{
		DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_HoldedItemDataTable.DT_HoldedItemDataTable'"));
	}
	
	
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerActors);

	UE_LOG(LogTemp,Error,TEXT("WidgetName : %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS HUDCLICK Tryget by Tag"));
	

	if (PlayerActors.Num() > 0)
	{
		AActor* PlayerActor = PlayerActors[0];
		UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS HUDCLICK: %s"), *PlayerActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS No actors with 'Player' tag found"));
	}
}
