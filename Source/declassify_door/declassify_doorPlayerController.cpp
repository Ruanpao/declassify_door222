// Copyright Epic Games, Inc. All Rights Reserved.


#include "declassify_doorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UI/PlayerHUD.h"
#include "Runtime\Engine\Classes\Engine\TimerHandle.h"
#include "CoreMinimal.h"

void Adeclassify_doorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &Adeclassify_doorPlayerController::InitializeHUD, 0.1f, false);

	
}

void Adeclassify_doorPlayerController::InitializeHUD()
{
	MyHUD = Cast<APlayerHUD>(GetHUD());

	UE_LOG(LogTemp, Warning, TEXT("InitializeHUD called in PlayerController"));
UE_LOG(LogTemp, Warning, TEXT("MyHUD is %s"), MyHUD ? TEXT("valid") : TEXT("null"));
	
	MyPlayer = Cast<Adeclassify_doorCharacter>(GetPawn());

	if(MyHUD && MyPlayer)
	{
		
	}
}
