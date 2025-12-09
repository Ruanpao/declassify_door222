// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "KeyTeleportDoor.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API AKeyTeleportDoor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AKeyTeleportDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	// 设置传送目标位置
	UFUNCTION(BlueprintCallable,Category="Teleport")
	void SetTeleportTarget(const FVector& NewTargetLocation);

	// 获取传送目标位置
	UFUNCTION(BlueprintCallable,Category="Teleport")
	FVector GetTeleportTarget() const{return TeleportTargetLocation;}

private:
	UPROPERTY(EditAnywhere,Category="Teleport")
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport", meta = (AllowPrivateAccess = "true"))
	FVector TeleportTargetLocation;

	bool bCanTeleport = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* DoorLockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* TeleportSound;

	UFUNCTION()
	void OnKeyPickedUp();
};
