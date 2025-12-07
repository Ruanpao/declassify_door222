// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "Engine/Canvas.h"
#include "UI/InventoryMainWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/DisposalPopWidget.h"

#include "declassify_door/declassify_doorPlayerController.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "UI/InteractPopWidget.h"


DEFINE_LOG_CATEGORY_STATIC(LogHUD, All, All);

void APlayerHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
    //UE_LOG(LogTemp,Error,TEXT("%s Try AddUObject"), *GetName());
    
}

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();



    
    
    auto PlayerDetailWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerDetailWidgetClass);
    
    auto LevelWidget = CreateWidget<UUserWidget>(GetWorld(), LevelWidgetClass);

    auto HoldedItemWidget = CreateWidget<UUserWidget>(GetWorld(), HoldedItemWidgetClass);
	
    InventoryInformationWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryInformationWidgetClass);

    InventoryMainWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryMainWidgetClass);
    
    if(PlayerDetailWidget)
    {
        PlayerDetailWidget->AddToViewport();
    }

    if(LevelWidget)
    {
        LevelWidget->AddToViewport();
    }

    if(HoldedItemWidget)
    {
        HoldedItemWidget->AddToViewport();
    }

    if(InventoryInformationWidget)
    {
    	InventoryInformationWidget->SetVisibility(ESlateVisibility::Hidden);
        InventoryInformationWidget->AddToViewport();
    }
	
    
    if(InventoryMainWidget)
    {
        InventoryMainWidget->AddToViewport();

        if (UInventoryMainWidget* InventoryMainWidgetInstance = Cast<UInventoryMainWidget>(InventoryMainWidget))
        {
            InventoryMainWidgetInstance->Received_2.AddUObject(this, &APlayerHUD::ReceivedInfo);
            
            InventoryMainWidgetInstance->ReceivedRemove.AddUObject(this, &APlayerHUD::RemoveRequest);
        }
    }
}

void APlayerHUD::ReceivedInfo(int32 Index)
{
	OnHoledSlotChanged.Broadcast(Index);
}


void APlayerHUD::InventoryInformationVisibility()
{
	if (InventoryInformationWidget)
	{
		if(InventoryInformationWidget->IsVisible())
		{
			InventoryInformationWidget->SetVisibility(ESlateVisibility::Hidden);
			UpdateMouseSituation(-1);
		}
		else
		{
			InventoryInformationWidget->SetVisibility(ESlateVisibility::Visible);
			UpdateMouseSituation(1);
		}
	}
}

void APlayerHUD::RemoveRequest(int32 Index)
{
	RemoveIndex =  Index;

	DisposalPopWidget = CreateWidget<UUserWidget>(GetWorld(), DisposalPopWidgetClass);

	if(DisposalPopWidget)
	{
		DisposalPopWidget->AddToViewport();

		if(UDisposalPopWidget* DisposalPopWidgetInstance = Cast<UDisposalPopWidget>(DisposalPopWidget))
		{
			DisposalPopWidgetInstance->Remove_2.AddUObject(this, &APlayerHUD::RemoveRequest_2);
		}
	}
	
}

void APlayerHUD::RemoveRequest_2(bool RemoveAll)
{
	RemoveItem.Broadcast(RemoveIndex , RemoveAll , false);

	UE_LOG(LogHUD , Warning , TEXT("RemoveIndex : %d , RemoveAll : %s"), RemoveIndex, RemoveAll ? TEXT("true") : TEXT("false"));
}

void APlayerHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX* 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max , Center.Min + HalfLineSize,Center.Max, LineColor,LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor,LineThickness);
}

void APlayerHUD::CreateInteractPopWidget(bool IsClicked, bool IsNearTower, bool IsFullTower , bool IsFullLevel , bool IsIntheMidLine , FItemInInventory HoldedItem)
{
	if (InteractPopWidget && InteractPopWidget->IsInViewport())
	{
		InteractPopWidget->RemoveFromParent();
		InteractPopWidget = nullptr;
	}
	
	InteractPopWidget = CreateWidget<UUserWidget>(GetWorld(), InteractPopWidgetClass);

	if(InteractPopWidget)
	{
		if(UInteractPopWidget* InteractPopWidgetInstance = Cast<UPVZ3DInteractPopWidget>(InteractPopWidget))
		{
			InteractPopWidgetInstance->SendHoldedItem.Broadcast(HoldedItem);
			InteractPopWidgetInstance->WhetherClickedAndNearTower.Broadcast(IsClicked, IsNearTower, IsFullTower, IsFullLevel, IsIntheMidLine);
		}

		InteractPopWidget->AddToViewport();
	}
}

void APlayerHUD::UpdateMouseSituation(int32 Num)
{
	CurrentVisibleNum += Num;

	UE_LOG(LogHUD ,  Error , TEXT("%d") , CurrentVisibleNum);
	
	if(CurrentVisibleNum > 0)
	{
		OnMouseSituationChanged.Broadcast(true);
	}
	else
	{
		OnMouseSituationChanged.Broadcast(false);
	}
}


