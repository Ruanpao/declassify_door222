// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ClearButton.h"
#include "Components/StaticMeshComponent.h"

AClearButton::AClearButton()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	// 创建按钮网格体
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);
	
	// 启用碰撞
	ButtonMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ButtonMesh->SetGenerateOverlapEvents(true);
}


void AClearButton::BeginPlay()
{
	Super::BeginPlay();
	
}


void AClearButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClearButton::OnClick()
{
	if (OnButtonClicked.IsBound())
	{
		OnButtonClicked.Broadcast();
	}
}

