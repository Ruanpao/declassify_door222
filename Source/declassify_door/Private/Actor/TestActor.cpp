// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actor/TestActor.h"
#include "Components/SphereComponent.h"  // 可选：添加球形碰撞检测

// 在构造函数中添加球形碰撞组件
ATestActor::ATestActor()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 创建球形碰撞组件（可选，用于调试）
    USphereComponent* SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    RootComponent = SphereCollision;
    SphereCollision->InitSphereRadius(150.0f);
    SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ATestActor::BeginPlay()
{
    Super::BeginPlay();
}

void ATestActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 实现接口函数
void ATestActor::OnInteract_Implementation(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("TestActor: 被 %s 交互了！"), *GetNameSafe(Interactor));
}