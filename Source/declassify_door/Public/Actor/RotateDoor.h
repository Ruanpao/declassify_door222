// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "RotateDoor.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API ARotateDoor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ARotateDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category = "Door")
	void RotateDoor();

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	// 设置传送目标位置
	UFUNCTION(BlueprintCallable,Category="Teleport")
	void SetTeleportTarget(const FVector& NewTargetLocation);

	// 获取传送目标位置
	UFUNCTION(BlueprintCallable,Category="Teleport")
	FVector GetTeleportTarget() const{return TeleportTargetLocation;}

	UFUNCTION(BlueprintCallable,Category = "Door")
	bool IsDoorUpsideDown() const{return bIsUpsideDown;}

	UFUNCTION(BlueprintCallable,Category = "Door")
	void SetDoorColor(const FLinearColor& NewColor);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Door")
	float TeleportDistance = 200.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Door")
	bool bIsRotated = false;


	//关联密码墙
	UFUNCTION(BlueprintCallable,Category="Door|Pass")
	void SetLinkedHidePass(class AHidePass* NewHidePass);

	UFUNCTION(BlueprintCallable,Category="Door|Pass")
	class AHidePass* GetLinkedHidePass() const{return LinkedHidePass;}
	
	UFUNCTION(BlueprintCallable,Category="Door|Pass")
	void SetRequiredColor(const FLinearColor& Color) {	RequiredPaintColor = Color;	}

private:
	UPROPERTY(VisibleAnywhere,Category = "Door")
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* TeleportSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport", meta = (AllowPrivateAccess = "true"))
	FVector TeleportTargetLocation;

	
	bool bIsUpsideDown = false;

	//关联密码墙
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Door|Pass",meta=(AllowPrivateAccess="true"))
	class AHidePass* LinkedHidePass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Door|Pass",meta=(AllowPrivateAccess="true"))
	FLinearColor RequiredPaintColor = FLinearColor::Red;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Door|Pass",meta=(AllowPrivateAccess="true"))
	bool bHasPaint = false;
};
