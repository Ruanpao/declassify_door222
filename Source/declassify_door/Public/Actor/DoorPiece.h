// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "DoorPiece.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpenedDelegate);

UCLASS()
class DECLASSIFY_DOOR_API ADoorPiece : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ADoorPiece();

protected:
	virtual void BeginPlay() override;

	//旋转速度
	UPROPERTY(EditAnywhere,Category="DoorPiece")
	float RotationSpeed = 60.f;

	//浮动速度
	UPROPERTY(EditAnywhere,Category="DoorPiece")
	float FloatingSpeed = 2.0f;

	//浮动范围
	UPROPERTY(EditAnywhere,Category="DoorPiece")
	float FloatngRange = 20.f;
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(BlueprintAssignable,Category="DoorPiece")
	FOnDoorOpenedDelegate OnDoorOpened;

private:
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, Category="DoorPiece")
	class UStaticMeshComponent* PieceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PickupSound;
};
