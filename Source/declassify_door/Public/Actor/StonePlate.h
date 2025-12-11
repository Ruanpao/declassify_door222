// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "StonePlate.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API AStonePlate : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AStonePlate();

protected:
	virtual void BeginPlay() override;

public:	
	
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StonePlate")
	FName ItemID = "7"; // 默认红色石板，对应ID 7

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StonePlate")
	bool bIsInSlot = false; // 标记是否在槽位中

//private:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category ="StonePlate",meta =(AllowPrivateAccess="true"))
	class UStaticMeshComponent* PlateMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="StonePlate",meta =(AllowPrivateAccess="true"))
	FLinearColor PlateColor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="StonePlate",meta =(AllowPrivateAccess="true"))
	class USoundBase* PickupSound;
};
