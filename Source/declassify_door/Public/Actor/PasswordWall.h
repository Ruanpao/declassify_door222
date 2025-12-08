// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClickOnlyPasswordLock.h"
#include "GameFramework/Actor.h"
#include "PasswordWall.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API APasswordWall : public AActor
{
	GENERATED_BODY()
	
public:	
	APasswordWall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="PasswordWall")
	void SetupPassWordLock(AClickOnlyPasswordLock* LockToBind);

	UFUNCTION()
	void OnPasswordLockUnlocked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PasswordWall")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PasswordWall")
	float TargetHeight = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PasswordWall")
	AClickOnlyPasswordLock* TargetLock;

private:
	UPROPERTY(EditAnywhere, Category="PasswordWall")
	class UStaticMeshComponent* WallMesh;

	bool bShouldMove = false;

	FVector StartLocation;
	FVector TargetLocation;
};
