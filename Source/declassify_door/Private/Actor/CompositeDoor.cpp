// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CompositeDoor.h"
#include "Components/StaticMeshComponent.h"
#include "./declassify_door/declassify_doorCharacter.h"
#include "Kismet/GameplayStatics.h"

ACompositeDoor::ACompositeDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	
	CurrentColor = FLinearColor::White;
	FirstColor = FLinearColor::White;
	PaintCount = 0;
	DoorMaterial = nullptr;
}

void ACompositeDoor::BeginPlay()
{
	Super::BeginPlay();

	if (DoorMesh)
	{
		InitialDoorRotation = DoorMesh->GetRelativeRotation();
	}
}

void ACompositeDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingOpen)
	{
		UpdateDoorRotation(DeltaTime);
	}
}

void ACompositeDoor::OnInteract_Implementation(AActor* Interactor)
{
	Adeclassify_doorCharacter*Player = Cast<Adeclassify_doorCharacter>(Interactor);
	if(Player && Player->bHasPaint)
	{
		PaintWithColor(Player->CurrentPaintColor);
	}
}

void ACompositeDoor::PaintWithColor(FLinearColor NewColor)
{
	if(PaintCount == 0)
	{
		FirstColor = NewColor;
		CurrentColor = NewColor;
		PaintCount++;
	}
	else if(PaintCount == 1)
	{
		CurrentColor = MixColors(FirstColor, NewColor);
		PaintCount++;
	}
	else
	{
		CurrentColor = FLinearColor::White;
		PaintCount = 0;
	}

	UpdateColor();
	
	if (ColorsMatch(CurrentColor, TargetColor, ColorMatchTolerance))
	{
		bTargetColorReached = true;
		
		if(SuccedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SuccedSound, GetActorLocation());
		}
		RotateDoorOpen();
	}
	
}

void ACompositeDoor::SetTargetColor(FLinearColor NewTargetColor)
{
	TargetColor = NewTargetColor;
	bTargetColorReached = false;
}

FLinearColor ACompositeDoor::MixColors(FLinearColor Color1, FLinearColor Color2)
{
	FLinearColor Result;
	Result.R = FMath::Max(Color1.R, Color2.R);
	Result.G = FMath::Max(Color1.G, Color2.G);
	Result.B = FMath::Max(Color1.B, Color2.B);
	Result.A = 1.0f;
	
	// 判断颜色类型
	bool IsRed = Color1.R > 0.8f || Color2.R > 0.8f;
	bool IsGreen = Color1.G > 0.8f || Color2.G > 0.8f;
	bool IsBlue = Color1.B > 0.8f || Color2.B > 0.8f;
	
	// 三原色混合规则
	if (IsRed && IsGreen && !IsBlue) return FLinearColor(1, 1, 0, 1);    // 黄色
	if (IsRed && IsBlue && !IsGreen) return FLinearColor(1, 0, 1, 1);    // 品红
	if (IsGreen && IsBlue && !IsRed) return FLinearColor(0, 1, 1, 1);    // 青色
	if (IsRed && IsGreen && IsBlue) return FLinearColor(1, 1, 1, 1);     // 白色
	
	return Result;
}

bool ACompositeDoor::ColorsMatch(FLinearColor Color1, FLinearColor Color2, float Tolerance)
{
	bool bMatch = FMath::Abs(Color1.R - Color2.R) <= Tolerance &&
				  FMath::Abs(Color1.G - Color2.G) <= Tolerance &&
				  FMath::Abs(Color1.B - Color2.B) <= Tolerance;

	return bMatch;
}

void ACompositeDoor::UpdateColor()
{
	if(bTargetColorReached)
	{
		return;
	}
	
	if (!DoorMesh)
	{
		return;
	}
	
	int32 MaterialIndex = 1;
	
	UMaterialInstanceDynamic* DynamicMaterial = DoorMesh->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
	if(DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("BaseColorFactor"), CurrentColor);
		if(PaintSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PaintSound, GetActorLocation());
		}
	}
}

void ACompositeDoor::RotateDoorOpen()
{
	if (bTargetColorReached && !bIsRotatingOpen)
	{
		bIsRotatingOpen = true;
		CurrentRotationAngle = 0.0f;
		
		if (DoorOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
		}
	}
}

void ACompositeDoor::UpdateDoorRotation(float DeltaTime)
{
	if (!DoorMesh || CurrentRotationAngle >= TargetRotationAngle)
	{
		bIsRotatingOpen = false;
		return;
	}
	
	float RotateDelta = RotateSpeed * DeltaTime;
	
	if (CurrentRotationAngle + RotateDelta > TargetRotationAngle)
	{
		RotateDelta = TargetRotationAngle - CurrentRotationAngle;
	}
	
	FRotator NewRotation = DoorMesh->GetRelativeRotation();
	
	if (RotationAxis.Yaw != 0)
	{
		NewRotation.Yaw += RotateDelta;
	}
	if (RotationAxis.Pitch != 0)
	{
		NewRotation.Pitch += RotateDelta;
	}
	if (RotationAxis.Roll != 0)
	{
		NewRotation.Roll += RotateDelta;
	}
	
	DoorMesh->SetRelativeRotation(NewRotation);
	
	CurrentRotationAngle += RotateDelta;
	
	if (CurrentRotationAngle >= TargetRotationAngle)
	{
		bIsRotatingOpen = false;
	}
}

