// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextActor.generated.h"

class UTextRenderComponent;


UCLASS()
class DECLASSIFY_DOOR_API ATextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATextActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere,Category = "Text")
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere,Category = "Text")
	UStaticMeshComponent* TextMesh;

	UPROPERTY(VisibleAnywhere,Category = "Text")
	UTextRenderComponent* ActorText;
};
