// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TextActor.h"

#include "Components/TextRenderComponent.h"


ATextActor::ATextActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	TextMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TextMesh"));
	TextMesh->SetupAttachment(RootComponent);

	ActorText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ActorText"));
	ActorText->SetupAttachment(RootComponent);
	ActorText->SetHorizontalAlignment(EHTA_Center);
	ActorText->SetRelativeLocation(FVector(0, 0, 100));
}

void ATextActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

