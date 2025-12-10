// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Pillar.h"
#include "TimerManager.h"
#include "Actor/SlotActor.h"
#include "Kismet/GameplayStatics.h"

APillar::APillar()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	PillarMesh->SetupAttachment(RootComponent);

	PillarText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PillarText"));
	PillarText->SetupAttachment(RootComponent);
	PillarText->SetHorizontalAlignment(EHTA_Center);
	PillarText->SetRelativeLocation(FVector(0, 0, 100));

	
}

void APillar::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = PillarMesh->GetRelativeLocation();
	TargetLocation = StartLocation - FVector(0.0f, 0.0f, 500);

	TArray<AActor*> FoundSlots;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASlotActor::StaticClass(), FoundSlots);

	for(AActor* Slot : FoundSlots)
	{
		ASlotActor* SlotActor = Cast<ASlotActor>(Slot);
		if(SlotActor)
		{
			SlotActor->OnPlatePlaced.AddDynamic(this,&APillar::ReceivePlateColor);
			SlotActor->OnPlateNumber.AddDynamic(this,&APillar::UpdateAnsNumber);
		}
	}
}

void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APillar::ReceivePlateColor(FLinearColor Color)
{
	if(bUnlocked || ReceivedColors.Num() >= 4)
	{
		return;
	}

	ReceivedColors.Add(Color);

	if(ReceivedColors.Num() == 4)
	{
		bool bCorrect = true;
		for(int32 i = 0; i < 4; i++)
		{
			if(!ReceivedColors[i].Equals(TargetColors[i],0.1f))
			{
				bCorrect = false;
				break;
			}
		}
		if(bCorrect && bIsNumber)
		{
			bUnlocked = true;

			StartLowering();
		}
	}
}

void APillar::StartLowering()
{
	bLowering = true;
	LowerProgress = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(LowerTimer,this,&APillar::UpdateLowering,0.02f,true);
}

void APillar::UpdateLowering()
{
	LowerProgress += 0.02f;

	if(LowerProgress >= 2.0f)
	{
		PillarMesh->SetRelativeLocation(TargetLocation);
		GetWorld()->GetTimerManager().ClearTimer(LowerTimer);
		return;
	}

	float Alpha = LowerProgress / 2.0f;
	FVector NewLocation = FMath::Lerp(StartLocation,TargetLocation,Alpha);
	PillarMesh->SetRelativeLocation(NewLocation);
}

void APillar::UpdateAnsNumber(int32 Number)
{
	AnsNumber +=Number;
	UE_LOG(LogTemp, Warning, TEXT("AnsNumber: %d, Target Ans: %d"), AnsNumber, Ans);
	
	if(Ans == AnsNumber)
	{
		bIsNumber = true;
	}
	else
	{
		bIsNumber = false;
	}
}

