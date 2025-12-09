// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "PaintBucket.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API APaintBucket : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	APaintBucket();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paint")
	FLinearColor PaintColor = FLinearColor::Red;

private:
	UPROPERTY(VisibleAnywhere,Category = "Paint")
	class UStaticMeshComponent* BucketMesh;
};
