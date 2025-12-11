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
			SlotActor->OnPlateRemoved.AddDynamic(this,&APillar::RemovePlateColor);
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
        UE_LOG(LogTemp, Warning, TEXT("石板数量大于四个"));
        return;
    }
	
    for(int32 i = 0; i < ReceivedColors.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("  [%d]: R=%.3f, G=%.3f, B=%.3f, A=%.3f"), 
            i, ReceivedColors[i].R, ReceivedColors[i].G, ReceivedColors[i].B, ReceivedColors[i].A);
    }
	
    for(int32 i = 0; i < TargetColors.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("  [%d]: R=%.3f, G=%.3f, B=%.3f, A=%.3f"), 
            i, TargetColors[i].R, TargetColors[i].G, TargetColors[i].B, TargetColors[i].A);
    }
    
    ReceivedColors.Add(Color);

    if(ReceivedColors.Num() == 4)
    {
        
        bool bCorrect = true;
        for(int32 i = 0; i < 4; i++)
        {
            FLinearColor received = ReceivedColors[i];
            FLinearColor target = TargetColors[i];
            
            // 计算颜色差异
            float rDiff = FMath::Abs(received.R - target.R);
            float gDiff = FMath::Abs(received.G - target.G);
            float bDiff = FMath::Abs(received.B - target.B);
            float totalDiff = rDiff + gDiff + bDiff;
            
            bool bMatches = ReceivedColors[i].Equals(TargetColors[i], 0.1f);

            if(!bMatches)
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
	if(PillarLowerSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,PillarLowerSound,GetActorLocation(),1.0f,1.0f,1.0f);
	}
	bLowering = true;
	LowerProgress = 0.0f;

	
	GetWorld()->GetTimerManager().SetTimer(LowerTimer,this,&APillar::UpdateLowering,0.04f,true);
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

void APillar::RemovePlateColor(FLinearColor Color)
{
	if(bUnlocked)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("当前石柱存储的颜色数量: %d"), ReceivedColors.Num());

	for(int32 i = 0; i < ReceivedColors.Num(); i++)
	{
		FLinearColor& StoredColor = ReceivedColors[i];
		UE_LOG(LogTemp, Warning, TEXT("颜色  [%d]: R=%.3f G=%.3f B=%.3f A=%.3f"), 
			i, StoredColor.R, StoredColor.G, StoredColor.B, StoredColor.A);
		
		// 显示颜色比较结果
		float Difference = FMath::Abs(StoredColor.R - Color.R) + 
						  FMath::Abs(StoredColor.G - Color.G) + 
						  FMath::Abs(StoredColor.B - Color.B);
		bool bMatches = StoredColor.Equals(Color, 0.1f);
		UE_LOG(LogTemp, Warning, TEXT(" 颜色     与目标颜色差异: %.3f, 是否匹配(容差0.1): %s"), 
			Difference, bMatches ? TEXT("是") : TEXT("否"));
	}

	
	for(int32 i = 0; i < ReceivedColors.Num(); i++)
	{
		if(ReceivedColors[i].Equals(Color, 0.1f))
		{
			UE_LOG(LogTemp, Warning, TEXT("移除石板"));
			ReceivedColors.RemoveAt(i);
			break;
		}
	}
	
}

