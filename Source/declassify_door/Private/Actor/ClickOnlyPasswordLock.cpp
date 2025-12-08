// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ClickOnlyPasswordLock.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/ClickableNumberButton.h"

AClickOnlyPasswordLock::AClickOnlyPasswordLock()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PassWordLockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PassWordLockMesh"));
	PassWordLockMesh->SetupAttachment(RootComponent);

	PassWordLockMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PassWordLockMesh->SetGenerateOverlapEvents(true);
}

void AClickOnlyPasswordLock::OnButtonClicked(int32 ButtonNumber)
{
	if(bIsUnlocked)
	{
		return;
	}

	if(ButtonNumber<0||ButtonNumber>9)
	{
		return;
	}

	//添加答案
	CurrentPassword.AppendInt(ButtonNumber);
	//播放音效
	PlaySound(ClickSound);

	if(CurrentPassword.Len() == PasswordLength)
	{
		ConfirmButton();
	}
}

void AClickOnlyPasswordLock::ClearButton()
{
	if(bIsUnlocked)
	{
		return;
	}
	//清空当前数字
	CurrentPassword="";
	PlaySound(ClickSound);
	
}

void AClickOnlyPasswordLock::ConfirmButton()
{
	if(bIsUnlocked)
	{
		return;
	}

	if(CurrentPassword.Len()!=PasswordLength)
	{
		PlaySound(WrongSound);
		CurrentPassword="";
		return;
	}

	if(CurrentPassword==CorrectPassword)
	{
		bIsUnlocked=true;
		PlaySound(SuccessSound);

		if(OnUnlocked.IsBound())
		{
			OnUnlocked.Broadcast();
		}
		
	}
	else
	{
		PlaySound(WrongSound);
		CurrentPassword="";
	}
}


void AClickOnlyPasswordLock::BeginPlay()
{
	Super::BeginPlay();
	CurrentPassword = "";
	bIsUnlocked = false;

	TArray<AActor*> FoundButtons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AClickableNumberButton::StaticClass(), FoundButtons);
	
	for (AActor* ButtonActor : FoundButtons)
	{
		if (AClickableNumberButton* Button = Cast<AClickableNumberButton>(ButtonActor))
		{
			Button->OnButtonClicked.AddDynamic(this, &AClickOnlyPasswordLock::OnButtonClicked);
		}
	}
	
}


void AClickOnlyPasswordLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClickOnlyPasswordLock::PlaySound(USoundBase* SoundToPlay)
{
	if(SoundToPlay && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(this,SoundToPlay,GetActorLocation());
	}
}

