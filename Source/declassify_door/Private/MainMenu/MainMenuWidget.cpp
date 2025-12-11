#include "Mainmenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MainMenu/MainMenuPlayerController.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget NativeConstruct called"));

    // 绑定按钮点击事件
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
        UE_LOG(LogTemp, Warning, TEXT("StartGameButton bound successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StartGameButton is null!"));
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGameClicked);
        UE_LOG(LogTemp, Warning, TEXT("QuitGameButton bound successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("QuitGameButton is null!"));
    }
}

void UMainMenuWidget::StartGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Start Game called via function"));
    OnStartGameClicked();
}

void UMainMenuWidget::QuitGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Quit Game called via function"));
    OnQuitGameClicked();
}

void UMainMenuWidget::OnStartGameClicked()
{
    PlayButtonSound();
    UE_LOG(LogTemp, Warning, TEXT("Start Game button clicked"));
    

    UGameplayStatics::OpenLevel(GetWorld(), "FirstMap"); // 替换为你的游戏地图
}

void UMainMenuWidget::OnQuitGameClicked()
{
    PlayButtonSound();
    UE_LOG(LogTemp, Warning, TEXT("Quit Game button clicked"));

    // 获取PlayerController并调用退出游戏
    APlayerController* PlayerController = GetOwningPlayer();
    if (AMainMenuPlayerController* MenuPC = Cast<AMainMenuPlayerController>(PlayerController))
    {
        MenuPC->QuitGame();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to MainMenuPlayerController"));
        // 备用方案：直接退出游戏
        UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
    }
}

void UMainMenuWidget::PlayButtonSound()
{
    if (ButtonClickSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
    }
}