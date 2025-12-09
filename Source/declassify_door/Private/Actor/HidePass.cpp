// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HidePass.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"

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

		
	}
}

void AHidePass::SetWallVisible(bool bVisible)
{
	WallMesh->SetVisibility(bVisible);
	WallMesh->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);

	TextRenderer->SetVisibility(bVisible);
}

