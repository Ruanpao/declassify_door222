// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClickableNumberButton.h"
#include "GameFramework/Actor.h"
#include "ClickOnlyPasswordLock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlockedDelegate);

UCLASS()
class DECLASSIFY_DOOR_API AClickOnlyPasswordLock : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AClickOnlyPasswordLock();

	//按钮被点击时
	UFUNCTION(BlueprintCallable,Category="PasswordLock")
	void OnButtonClicked(int32 ButtonNumber);

	//清除按钮输入
	UFUNCTION(BlueprintCallable,Category="PasswordLock")
	void ClearButton();

	//确认按钮输入
	UFUNCTION(BlueprintCallable,Category="PasswordLock")
	void ConfirmButton();

	//返回输入密码
	UFUNCTION(BlueprintCallable,Category="PasswordLock")
	FString GetCurrentPassword() const { return CurrentPassword; };

	UFUNCTION(BlueprintCallable,Category="PasswordLock")
	bool IsUnLocked() const { return bIsUnlocked; };
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//密码
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PasswordLock")
	FString CorrectPassword = "1145";

	//音效
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PasswordLock")
	class USoundBase * ClickSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PasswordLock")
	USoundBase* WrongSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PasswordLock")
	USoundBase* SuccessSound;

	UPROPERTY(BlueprintAssignable,Category="PasswordLock")
	FOnUnlockedDelegate OnUnlocked;

private:
	FString CurrentPassword;

	bool bIsUnlocked = false;

	static const int32 PasswordLength = 4;

	void PlaySound(USoundBase* SoundToPlay);

	UPROPERTY(EditAnywhere,Category="Button")
	class UStaticMeshComponent* PassWordLockMesh;
};
