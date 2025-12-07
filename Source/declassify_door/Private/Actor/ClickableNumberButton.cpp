// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ClickableNumberButton.h"

#include "Kismet/GameplayStatics.h"


AClickableNumberButton::AClickableNumberButton()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);

	ButtonMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ButtonMesh->SetGenerateOverlapEvents(true);

}

void AClickableNumberButton::BeginPlay()
{
	Super::BeginPlay();
}

void AClickableNumberButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClickableNumberButton::Onclick()
{
	if(PressSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(this,PressSound,GetActorLocation());
	}

	OnButtonClicked.Broadcast(ButtonNumber);
}


