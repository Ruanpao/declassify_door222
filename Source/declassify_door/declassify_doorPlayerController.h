// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "declassify_doorCharacter.h"
#include "UI/PlayerHUD.h"
#include "declassify_doorPlayerController.generated.h"

class UInputMappingContext;
class UPauseMenuWidget;

UCLASS()
class DECLASSIFY_DOOR_API Adeclassify_doorPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// 暂停菜单相关
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause Menu")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;
    
	UPROPERTY()
	UPauseMenuWidget* PauseMenuWidget;
    
	UPROPERTY(BlueprintReadOnly, Category = "Pause Menu")
	bool bIsPaused;

	// Begin Actor interface
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	APlayerHUD* MyHUD;
	Adeclassify_doorCharacter* MyPlayer;
    
	void InitializeHUD();
	FTimerHandle TimerHandle;

	// 暂停功能
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void TogglePauseMenu();
    
	void ShowPauseMenu();
	void HidePauseMenu();

	// End Actor interface
};