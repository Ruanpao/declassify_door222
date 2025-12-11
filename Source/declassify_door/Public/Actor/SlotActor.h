// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/StonePlate.h"
#include "Interface/InteractInterface.h"
#include "SlotActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlatePlaced, FLinearColor, PlateColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlateNumber, int32, Number);

UCLASS()
class DECLASSIFY_DOOR_API ASlotActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ASlotActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(BlueprintAssignable,Category="SlotStone")
	FOnPlatePlaced OnPlatePlaced;

	UPROPERTY(BlueprintAssignable,Category="SlotStone")
	FOnPlateNumber OnPlateNumber;
	
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SlotStone",meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* SlotMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SlotStone",meta=(AllowPrivateAccess="true"))
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SlotStone",meta=(AllowPrivateAccess="true"))
	class UTextRenderComponent* SlotText;

	UPROPERTY(EditAnywhere,Category = "SlotStone")
	FVector PlateOffset = FVector(0.0f,0.0f,10.0f);

	UPROPERTY(BlueprintReadOnly,Category = "SlotStone",meta=(AllowPrivateAccess="true"))
	AStonePlate* CurrentPlate = nullptr;
	
	UPROPERTY(EditAnywhere,Category = "SlotStone")
	int32 AnsNumber =0 ;

	UPROPERTY(EditAnywhere,Category = "SlotStone")
	class USoundBase * PutSound;
};
