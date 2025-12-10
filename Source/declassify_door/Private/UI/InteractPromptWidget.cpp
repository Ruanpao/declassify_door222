// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractPromptWidget.h"
#include "Components/TextBlock.h"

void UInteractPromptWidget::SetInteractText(const FText& Text)
{
	if (InteractText)
	{
		InteractText->SetText(Text);
	}
}