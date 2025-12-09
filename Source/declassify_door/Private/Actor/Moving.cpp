// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Moving.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

AMoving::AMoving()
{
	PrimaryActorTick.bCanEverTick = true;

	// 创建根组件
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// 创建石板网格
	MovingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	MovingMesh->SetupAttachment(RootComponent);
	
	MovingMesh->SetSimulatePhysics(false);
	MovingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MovingMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	MoveSpeed = 100.0f; 
	MoveDistance = 500.0f; 
}

void AMoving::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	ForwardVector = GetActorForwardVector();
	
}

void AMoving::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if(bCanMove && bIsMoving)
	{
		float MoveDelta = MoveSpeed * DeltaTime;
	
		FVector Movement = ForwardVector  * MoveDelta * MoveDirection;

		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation + Movement;

		CurrentSegmentDistance += MoveDelta;

		SetActorLocation(NewLocation);

		if(CurrentSegmentDistance >= MoveDistance)
		{
			MoveDirection *= -1;
			CurrentSegmentDistance = 0.0f;
		}
	}

	if(bCanRotate && bIsRotating)
	{
		// float RotateDelta = RotateSpeed * DeltaTime;
		//
		// float RemainingAngle = TargetRotationAngle - CurrentRotationAngle;
		// float ActualRotateDelta = FMath::Min(RotateDelta, RemainingAngle);
		//
		// FRotator NewRotation = GetActorRotation();
		// NewRotation.Yaw += ActualRotateDelta * RotateDirection;
		//
		// SetActorRotation(NewRotation);
		//
		// CurrentRotationAngle += ActualRotateDelta;
		//
		// if(CurrentRotationAngle >= TargetRotationAngle)
		// {
		// 	bIsRotating = false;
		// 	CurrentRotationAngle = 0.0f;
		// 	RotateDirection *= -1;
		// }

		float RotateDelta = RotateSpeed * DeltaTime * RotateDirection;
		
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += RotateDelta;

		SetActorRotation(NewRotation);
	}
}

void AMoving::StartMoving()
{
	bIsMoving = true;
}

void AMoving::StopMoving()
{
	bIsMoving = false;
}

void AMoving::ResetPosition()
{
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
	MoveDirection = 1;
	CurrentSegmentDistance = 0.0f;
	CurrentRotationAngle = 0.0f;
	RotateDirection = 1;
	bIsMoving = false;
	bIsRotating = false;
}

void AMoving::StartRotating()
{
	if(bCanRotate)
	{
		bIsRotating = true;
		CurrentRotationAngle = 0.0f;
		RotateDirection = 1;
	}
}

void AMoving::StopRotating()
{
	bIsRotating = false;
}

