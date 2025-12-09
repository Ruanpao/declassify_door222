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
	
private:
	UPROPERTY(VisibleAnywhere,Category = "Moving")
	class UStaticMeshComponent* MovingMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	float MoveDistance = 500.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	int32 MoveDirection = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Moving",meta = (AllowPrivateAccess = "true"))
	float CurrentSegmentDistance = 0.0f;

	FVector StartLocation;

	FVector ForwardVector;
};
