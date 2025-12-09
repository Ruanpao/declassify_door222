// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PaintBucket.h"
#include "Components/StaticMeshComponent.h"
#include "declassify_door/declassify_doorCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PlayerHUD.h"


APaintBucket::APaintBucket()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BucketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BucketMesh"));
	BucketMesh->SetupAttachment(RootComponent);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(300.0f); // 设置检测半径
	InteractionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 设置碰撞配置文件
}

void APaintBucket::BeginPlay()
{
	Super::BeginPlay();
	if (InteractionSphere)
	{
		InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &APaintBucket::OnSphereBeginOverlap);
		InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &APaintBucket::OnSphereEndOverlap);
	}
}

void APaintBucket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APaintBucket::OnInteract_Implementation(AActor* Interactor)
{
	if(!Interactor)
	{
		return;
	}

	if(ACharacter* Character = Cast<ACharacter>(Interactor))
	{
		if(Character->IsA(Adeclassify_doorCharacter::StaticClass()))
		{
			Adeclassify_doorCharacter* MyCharacter = Cast<Adeclassify_doorCharacter>(Character);
			// if(MyCharacter)
			// {
			// 	MyCharacter->PickupPaint(PaintColor);
			// }
			if(MyCharacter->InventoryComponent)
			{
				if(PaintColor==FLinearColor::Red)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("2"),1);
				}
				else if(PaintColor==FLinearColor::Green)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("3"),1);
				}
				else if(PaintColor==FLinearColor::Blue)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("4"),1);
				}
				else if(PaintColor==FLinearColor::White)
				{
					MyCharacter->InventoryComponent->AddToInventory(FName("5"),1);
				}
			}
		}
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		Destroy();
	}
}

FText APaintBucket::GetInteractText() const
{
	if(PaintColor==FLinearColor::Red){
		return FText::FromString("Press E to pickup paintbucket Red");
	}
	else if(PaintColor==FLinearColor::Blue)
	{
		return FText::FromString("Press E to pickup paintbucket Blue");
	}
	else if(PaintColor==FLinearColor::Green)
	{
		return FText::FromString("Press E to pickup paintbucket Green");
	}
	else if(PaintColor==FLinearColor::White)
	{
		return FText::FromString("Press E to pickup paintbucket White");
	}
	else
	{
		return FText::FromString("Press E to pickup paintbucket");
	}
}


// 新增：碰撞开始处理函数
void APaintBucket::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Adeclassify_doorCharacter* Player = Cast<Adeclassify_doorCharacter>(OtherActor);
	if (Player && Player->Controller && Player->Controller->IsPlayerController())
	{
		APlayerController* PlayerController = Cast<APlayerController>(Player->Controller);
		if (PlayerController)
		{
			APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (HUD)
			{
				FText InteractText = GetInteractText();
				HUD->ShowInteractPrompt(InteractText);
				UE_LOG(LogTemp, Warning, TEXT("Player entered interaction sphere, showing prompt"));
			}
		}
	}
}

void APaintBucket::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Adeclassify_doorCharacter* Player = Cast<Adeclassify_doorCharacter>(OtherActor);
	if (Player && Player->Controller && Player->Controller->IsPlayerController())
	{
		APlayerController* PlayerController = Cast<APlayerController>(Player->Controller);
		if (PlayerController)
		{
			APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (HUD)
			{
				HUD->HideInteractPrompt();
				UE_LOG(LogTemp, Warning, TEXT("Player left interaction sphere, hiding prompt"));
			}
		}
	}
}
