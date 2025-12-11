// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../declassify_doorPlayerController.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnContinueClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
	}
}

void UPauseMenuWidget::ContinueGame()
{
	Adeclassify_doorPlayerController* PlayerController = Cast<Adeclassify_doorPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->TogglePauseMenu();
	}
}

void UPauseMenuWidget::QuitGame()
{
	// 退出游戏
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
	UGameplayStatics::OpenLevel(GetWorld(), "MenuMap"); // 替换为你的游戏地图

}

void UPauseMenuWidget::OnContinueClicked()
{
	ContinueGame();
}

void UPauseMenuWidget::OnQuitClicked()
{
	QuitGame();
}
