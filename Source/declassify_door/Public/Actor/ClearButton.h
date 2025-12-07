// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClearButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClearButtonClickedDelegate);

UCLASS()
class DECLASSIFY_DOOR_API AClearButton : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AClearButton();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Button")
	void OnClick();
	
	
	UPROPERTY(BlueprintAssignable,Category="Button")
	FClearButtonClickedDelegate OnButtonClicked;

private:
	// 按钮网格体
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UStaticMeshComponent* ButtonMesh;
};
