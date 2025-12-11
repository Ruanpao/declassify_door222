// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Pillar.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API APillar : public AActor
{
	GENERATED_BODY()
	
public:	
	APillar();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category = "Pillar")
	void ReceivePlateColor(FLinearColor Color);

	void StartLowering();

	void UpdateLowering();

	UFUNCTION(BlueprintCallable,Category = "Pillar")
	void UpdateAnsNumber(int32 Number);
	
private:
	UPROPERTY(VisibleAnywhere,Category = "Pillar")
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere,Category = "Pillar")
	UStaticMeshComponent* PillarMesh;

	UPROPERTY(VisibleAnywhere,Category = "Pillar")
	UTextRenderComponent* PillarText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Pillar",meta = (AllowPrivateAccess = "true"))
	TArray<FLinearColor> TargetColors;
	
	TArray<FLinearColor> ReceivedColors;

	FTimerHandle LowerTimer;

	bool bUnlocked = false;

	bool bLowering = false;
	
	float LowerProgress = 0.0f;

	FVector StartLocation;
	
	FVector TargetLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Pillar",meta = (AllowPrivateAccess = "true"))
	int32 Ans;

	int32 AnsNumber = 0;	

	bool bIsNumber = false;

	UPROPERTY(EditAnywhere,Category="Pillar")
	class USoundBase* PillarLowerSound;
};
