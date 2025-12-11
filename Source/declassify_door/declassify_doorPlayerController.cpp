// Copyright Epic Games, Inc. All Rights Reserved.

#include "declassify_doorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UI/PlayerHUD.h"
#include "Runtime/Engine/Classes/Engine/TimerHandle.h"
#include "CoreMinimal.h"
#include "UI/PauseMenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void Adeclassify_doorPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // get the enhanced input subsystem
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // add the mapping context so we get controls
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &Adeclassify_doorPlayerController::InitializeHUD, 0.1f, false);

    // 初始化暂停状态
    bIsPaused = false;
    
    if (!PauseMenuWidget)
    {
        PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
        if (!PauseMenuWidget)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create pause menu widget"));
            return;
        }
        PauseMenuWidget->AddToViewport(1000); // 使用较高的ZOrder确保在最前面
        PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void Adeclassify_doorPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 绑定ESC键到暂停功能
    if (InputComponent)
    {
        InputComponent->BindKey(EKeys::BackSpace, IE_Pressed, this, &Adeclassify_doorPlayerController::TogglePauseMenu);
    }
}

void Adeclassify_doorPlayerController::InitializeHUD()
{
    MyHUD = Cast<APlayerHUD>(GetHUD());

    UE_LOG(LogTemp, Warning, TEXT("InitializeHUD called in PlayerController"));
    UE_LOG(LogTemp, Warning, TEXT("MyHUD is %s"), MyHUD ? TEXT("valid") : TEXT("null"));
    
    MyPlayer = Cast<Adeclassify_doorCharacter>(GetPawn());

    if(MyHUD && MyPlayer)
    {
        // HUD和玩家都已初始化，背包UI会自动持续显示
        UE_LOG(LogTemp, Warning, TEXT("HUD and Player initialized successfully"));
    }
}

void Adeclassify_doorPlayerController::TogglePauseMenu()
{
    if (bIsPaused)
    {
        HidePauseMenu();
        return;
    }
    else
    {
        ShowPauseMenu();
        return;
    }
}

void Adeclassify_doorPlayerController::ShowPauseMenu()
{
    if (!PauseMenuWidgetClass) return;

    // 创建暂停菜单
    
    PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);

    // 显示暂停菜单
    
    // 设置游戏暂停
    SetPause(true);
    bIsPaused = true;
    
    // 显示鼠标光标
    bShowMouseCursor = true;
    
    // 设置输入模式为UI和游戏同时响应
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    
    UE_LOG(LogTemp, Warning, TEXT("Pause menu shown"));
}

void Adeclassify_doorPlayerController::HidePauseMenu()
{
    if (PauseMenuWidget)
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    // 取消游戏暂停
    SetPause(false);
    bIsPaused = false;
    
    // 隐藏鼠标光标
    bShowMouseCursor = false;
    
    // 恢复游戏输入模式
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    
    UE_LOG(LogTemp, Warning, TEXT("Pause menu hidden"));
}