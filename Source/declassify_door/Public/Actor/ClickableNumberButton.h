// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClickableNumberButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClickedSignature, int32, ButtonNum);

UCLASS()
class DECLASSIFY_DOOR_API AClickableNumberButton : public AActor
{
	GENERATED_BODY()
	
public:	
	AClickableNumberButton();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	//被点击时
	void Onclick();

	void SetButtonNumber(int32 Number) {ButtonNumber = Number;}

	int32 GetButtonNumber() const { return ButtonNumber; }

	//按钮被点击事件
	UPROPERTY(BlueprintAssignable,Category="Button")
	FOnButtonClickedSignature OnButtonClicked;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Button")
	class USoundBase* PressSound;

private:
	UPROPERTY(EditAnywhere,Category="Button")
	int32 ButtonNumber=0;

	UPROPERTY(EditAnywhere,Category="Button")
	class UStaticMeshComponent* ButtonMesh;
	
};
