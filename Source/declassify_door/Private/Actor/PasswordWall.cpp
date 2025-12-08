// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PasswordWall.h"
#include "Kismet/GameplayStatics.h"

APasswordWall::APasswordWall()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);
	WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallMesh->SetGenerateOverlapEvents(true);
}


void APasswordWall::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + FVector(0,0,TargetHeight);
	bShouldMove = false;
	
	if(TargetLock)
	{
		TargetLock->OnUnlocked.AddDynamic(this, &APasswordWall::OnPasswordLockUnlocked);
	}
}


void APasswordWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldMove)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
		SetActorLocation(NewLocation);

		if(FVector::Dist(NewLocation, TargetLocation) < 1.0f)
		{
			bShouldMove = false;
		}
	}
}

void APasswordWall::SetupPassWordLock(AClickOnlyPasswordLock* LockToBind)
{
	if(LockToBind)
	{
		LockToBind->OnUnlocked.AddDynamic(this, &APasswordWall::OnPasswordLockUnlocked);
	}
}

void APasswordWall::OnPasswordLockUnlocked()
{
	bShouldMove = true;
}
