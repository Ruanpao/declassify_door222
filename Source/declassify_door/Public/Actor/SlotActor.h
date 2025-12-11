// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "SlotActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlatePlaced, FLinearColor, PlateColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlateNumber, int32, Number);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlateRemoved, FLinearColor, PlateColor);

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

	UPROPERTY(BlueprintAssignable,Category="SlotStone")
	FOnPlateRemoved OnPlateRemoved;

	UPROPERTY()
	FName StoredPlateID;

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SlotStone",meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* SlotMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SlotStone",meta=(AllowPrivateAccess="true"))
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SlotStone",meta=(AllowPrivateAccess="true"))
	class UTextRenderComponent* SlotText;

	// 动态材质实例
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterial;
	
	// 默认颜色
	UPROPERTY(EditAnywhere, Category = "SlotStone")
	FLinearColor DefaultColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f); // 灰色
	
	// 当前石板颜色
	UPROPERTY()
	FLinearColor CurrentPlateColor;
	
	// 标记是否有石板
	bool bHasPlate = false;

	UPROPERTY(EditAnywhere,Category = "SlotStone")
	int32 AnsNumber = 0;

	UPROPERTY(EditAnywhere,Category = "SlotStone")
	class USoundBase * PutSound;
};