// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractPromptWidget.generated.h"

/**
 * 
 */

UCLASS()
class DECLASSIFY_DOOR_API UInteractPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractText(const FText& Text);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractText;
};