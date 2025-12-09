// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Moving.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API AMoving : public AActor
{
	GENERATED_BODY()
	
public:	
	AMoving();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category = "Moving")
	void StartMoving();

	UFUNCTION(BlueprintCallable,Category = "Moving")
	void StopMoving();

	UFUNCTION(BlueprintCallable,Category = "Moving")
	void ResetPosition();

	UFUNCTION(BlueprintCallable,Category = "Moving")
	void SetCanMove(bool bCan)	{	bCanMove = bCan;}

	UFUNCTION(BlueprintCallable,Category = "Moving")
	void SetCanRotate(bool bCan){	bCanRotate= bCan;}

	UFUNCTION(BlueprintCallable,Category = "Moving")
	bool GetCanMove () const { return bCanMove; }

	UFUNCTION(BlueprintCallable,Category = "Moving")
	bool GetCanRotate () const { return bCanRotate; }

	// 开始旋转
	UFUNCTION(BlueprintCallable, Category = "Moving")
	void StartRotating();
	
	// 停止旋转
	UFUNCTION(BlueprintCallable, Category = "Moving")
	void StopRotating();

	
private:
	UPROPERTY(VisibleAnywhere,Category = "Moving")
	class UStaticMeshComponent* MovingMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	float MoveDistance = 500.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving|Rotation", meta = (AllowPrivateAccess = "true"))
	bool bIsRotating = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	int32 MoveDirection = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	float CurrentSegmentDistance = 0.0f;

	// 是否可以移动
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving|Control", meta = (AllowPrivateAccess = "true"))
	bool bCanMove = true;

	// 是否可以旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving|Control", meta = (AllowPrivateAccess = "true"))
	bool bCanRotate = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving|Rotation", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 30.0f;

	// 目标旋转角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving|Rotation", meta = (AllowPrivateAccess = "true"))
	float TargetRotationAngle = 180.0f;

	// 当前已旋转角度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving|Rotation", meta = (AllowPrivateAccess = "true"))
	float CurrentRotationAngle = 0.0f;

	// 旋转方向
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moving|Rotation", meta = (AllowPrivateAccess = "true"))
	int32 RotateDirection = 1;
	
	FVector StartLocation;

	FVector ForwardVector;

	FRotator StartRotation;
};
