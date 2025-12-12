// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HidePass.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

AHidePass::AHidePass()
{
	PrimaryActorTick.bCanEverTick = true;

	// 创建根组件
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// 创建墙体网格
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(WallMesh);
	TextRenderer->SetRelativeLocation(FVector(0,0,180.0f));
	TextRenderer->SetRelativeRotation(FRotator(0, 180.0f, 0)); 
	TextRenderer->SetHorizontalAlignment(EHTA_Center);
	TextRenderer->SetVerticalAlignment(EVRTA_TextCenter);
	TextRenderer->SetTextRenderColor(FColor::White);
	TextRenderer->SetWorldSize(50.0f);

	PassCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PassCamera"));
	PassCamera->SetupAttachment(RootComponent);
	PassCamera->SetRelativeLocation(FVector(100.0f, 0, 250.0f));  // 在文字前方100，上方250
	PassCamera->SetRelativeRotation(FRotator(-20.0f, 180.0f, 0)); // 稍微向下看，朝向文字
	PassCamera->FieldOfView = 60.0f;
}


void AHidePass::BeginPlay()
{
	Super::BeginPlay();

	SetWallVisible(false);
}


void AHidePass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHidePass::SetRequiredColor(const FLinearColor& Color)
{
	RequiredColor = Color;
}

void AHidePass::CheckAndReveal(const FLinearColor& DoorColor)
{
	if(bIsRevealed)
	{
		return;
	}

	bool bColorMatches = 
		FMath::Abs(DoorColor.R - RequiredColor.R) < ColorTolerance &&
		FMath::Abs(DoorColor.G - RequiredColor.G) < ColorTolerance &&
		FMath::Abs(DoorColor.B - RequiredColor.B) < ColorTolerance;
	
	if(bColorMatches)
	{
		SetWallVisible(true);
		bIsRevealed = true;

		UE_LOG(LogTemp, Log, TEXT("密码墙显示,门颜色匹配"));

		if(SucceedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SucceedSound, GetActorLocation());
		}
		
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (PlayerPawn)
		{
			StartCameraTransition(PlayerPawn);
		}
	}
}

void AHidePass::SetWallVisible(bool bVisible)
{
	WallMesh->SetVisibility(bVisible);
	WallMesh->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

	TextRenderer->SetVisibility(bVisible);
}


void AHidePass::StartCameraTransition(APawn* TargetPawn)
{
	if(!TargetPawn || !TargetPawn->GetController())
	{
		return;
	}

	OriginalPawn = TargetPawn;
	APlayerController* PC = Cast<APlayerController>(TargetPawn->GetController());
	if(!PC)
	{
		return;
	}

	// 直接从玩家摄像机切换到HidePass的摄像机
	PC->SetViewTargetWithBlend(this, CameraTransitionTime, VTBlend_Cubic, 1.0f, false);
	
	GetWorld()->GetTimerManager().SetTimer(CameraTimerHandle, this, &AHidePass::ReturnToPlayerCamera, CameraHoldTime + CameraTransitionTime, false);
}



void AHidePass::ReturnToPlayerCamera()
{
	if(OriginalPawn && OriginalPawn->GetController())
	{
		APlayerController* PC = Cast<APlayerController>(OriginalPawn->GetController());
		if(PC)
		{
			PC->SetViewTargetWithBlend(OriginalPawn, CameraTransitionTime, VTBlend_Cubic, 1.0f, false);
		}
	}
	OriginalPawn = nullptr;
}


