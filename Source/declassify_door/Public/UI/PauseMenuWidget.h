// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */

UCLASS()
class DECLASSIFY_DOOR_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ContinueGame();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void QuitGame();

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

private:
	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnQuitClicked();
};