// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "LeverActor.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API ALeverActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ALeverActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable,Category = "Lever" )
	bool IsLeverPulled() const { return bIsPulled; }

	UFUNCTION(BlueprintCallable,Category = "Lever" )
	void SetLinkedMovingActor(class AMoving* MovingActor);

private:
	UPROPERTY(VisibleAnywhere,Category = "Lever")
	class UStaticMeshComponent* LeverMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever", meta = (AllowPrivateAccess = "true"))
	bool bIsPulled = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lever", meta = (AllowPrivateAccess = "true"))
	float PullRotationAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lever", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PullSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Lever",meta = (AllowPrivateAccess = "true"))
	class AMoving* LinkedMovingActor;
};
