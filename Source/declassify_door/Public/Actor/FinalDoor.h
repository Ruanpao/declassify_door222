// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalDoor.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API AFinalDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AFinalDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere,Category = "FinalDoor")
	class UStaticMeshComponent* DoorMesh;

	int32 CurrentNumberOfKeys = 0;
};
