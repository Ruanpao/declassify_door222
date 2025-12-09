// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "KeyActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyPickedUpDelegate);

UCLASS()
class DECLASSIFY_DOOR_API AKeyActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AKeyActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(BlueprintAssignable,Category="Key")
	FOnKeyPickedUpDelegate OnKeyPickedUp;

private:
	UPROPERTY(EditAnywhere, Category="Key")
	class UStaticMeshComponent* KeyMesh;
};
