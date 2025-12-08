// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ClickOnlyPasswordLock.h"
#include "Kismet/GameplayStatics.h"

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
	if(CurrentPassword.Len()>= PasswordLength)
	{
		return;
	}

	//添加答案
	CurrentPassword.AppendInt(ButtonNumber);
	//播放音效
	PlaySound(ClickSound);
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

