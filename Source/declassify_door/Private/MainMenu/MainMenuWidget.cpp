#include "Mainmenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MainMenu/MainMenuPlayerController.h"
#include "EngineUtils.h" // 新增include
#include "Actor/CompositeDoor.h"
#include "Camera/CameraActor.h"
#include "Actor/CompositeDoor.h"

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


    bIsAnimating = false;
    AnimationTime = 0.0f;
    TotalAnimationTime = 3.0f; // 3秒动画
    CameraStartDistance = 0.0f;
    CameraTargetDistance = 2500.0f; // 摄像机前飞500单位
    
}


void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsAnimating)
    {
        UpdateAnimation(InDeltaTime);
    }
}

void UMainMenuWidget::StartAnimation()
{
    if (bIsAnimating) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // 查找旋转的Actor（查找第一个非摄像机的Actor）
    for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
    {
        AActor* Actor = *ActorItr;
        if (Actor && Actor->IsA<ACompositeDoor>())
        {
            RotatingActor = Actor;
            break;
        }
    }

    // 查找摄像机
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        CameraActor = PC->GetViewTarget();
        if (CameraActor)
        {
            CameraStartDistance = 0.0f;
        }
    }

    bIsAnimating = true;
    AnimationTime = 0.0f;

    UE_LOG(LogTemp, Warning, TEXT("Starting animation"));
}

void UMainMenuWidget::UpdateAnimation(float DeltaTime)
{
    AnimationTime += DeltaTime;
    float Alpha = FMath::Clamp(AnimationTime / TotalAnimationTime, 0.0f, 1.0f);

    // 旋转Actor
    if (RotatingActor)
    {
        FRotator NewRotation = RotatingActor->GetActorRotation();
        NewRotation.Yaw += 60.0f * DeltaTime; // 每秒旋转180度
        RotatingActor->SetActorRotation(NewRotation);
    }

    // 移动摄像机
    if (CameraActor)
    {
        FVector NewLocation = CameraActor->GetActorLocation();
        NewLocation.X += CameraTargetDistance * DeltaTime / TotalAnimationTime; // 向前移动
        NewLocation.Y += 500 * DeltaTime / TotalAnimationTime; // 向前移动

        CameraActor->SetActorLocation(NewLocation);
    }

    // 动画结束
    if (AnimationTime >= TotalAnimationTime)
    {
        bIsAnimating = false;
        UE_LOG(LogTemp, Warning, TEXT("Animation finished, loading level"));
        
        // 按照原来的逻辑加载新地图
        UGameplayStatics::OpenLevel(GetWorld(), "PlayGame");
    }
}
// === 新增代码结束 ===

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
    SetColorAndOpacity(FLinearColor(1, 1, 1, 0));

    SetColorAndOpacity(FLinearColor(1, 1, 1, 0));

    
    StartAnimation(); 
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