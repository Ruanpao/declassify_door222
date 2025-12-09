// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "CompositeDoor.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API ACompositeDoor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ACompositeDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable,Category = "Door")
	void PaintWithColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable,Category = "Door")
	void SetTargetColor(FLinearColor NewTargetColor);
	
	FLinearColor GetCurrentColor() const { return CurrentColor; }

	UFUNCTION(BlueprintCallable,Category = "Door")
	bool IsTargetColorReacjed() const {return bTargetColorReached; }

private:
	FLinearColor MixColors(FLinearColor Color1,FLinearColor Color2);

	bool ColorsMatch(FLinearColor Color1, FLinearColor Color2, float Tolerance = 0.1f);
	
	void UpdateColor();

	void RotateDoorOpen();
	void UpdateDoorRotation(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DoorMesh;

	UMaterialInstanceDynamic * DoorMaterial;

	FLinearColor FirstColor;
	FLinearColor CurrentColor;

	int PaintCount;

	// 目标颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
	FLinearColor TargetColor;
	
	// 是否已达到目标颜色
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
	bool bTargetColorReached;

	UPROPERTY(EditAnywhere, Category = "Door")
	float ColorMatchTolerance = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PaintSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* SuccedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* DoorOpenSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
	bool bIsRotatingOpen;
	

	UPROPERTY(EditAnywhere, Category = "Door Rotation")
	float RotateSpeed = 30.0f;  
	
	UPROPERTY(EditAnywhere, Category = "Door Rotation")
	float TargetRotationAngle = 90.0f;  
	
	UPROPERTY(VisibleAnywhere, Category = "Door Rotation")
	float CurrentRotationAngle;  
	
	UPROPERTY(EditAnywhere, Category = "Door Rotation")
	FRotator RotationAxis = FRotator(0, 1, 0);
	
	FRotator InitialDoorRotation;
};
