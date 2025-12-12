// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h" // 添加接口头文件
#include "FinalDoor.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API AFinalDoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
    
public: 
	AFinalDoor();

protected:
	virtual void BeginPlay() override;

public: 
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddKey();
    
	// 实现交互接口
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable,Category="FinalDoor")
	void StartCameraTransition(APawn* TargetPawn);
	
private:
	UFUNCTION()
	void ReturnToMainMenu();
    
	UPROPERTY(EditAnywhere, Category = "FinalDoor")
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere,Category="FinalDoor")
	class UCameraComponent* FinalCamera;

	int32 CurrentNumberOfKeys = 0;

	UPROPERTY(EditAnywhere, Category = "FinalDoor")
	class USoundBase* DoorCreateSound;
    
	UPROPERTY(EditAnywhere, Category = "FinalDoor")
	class USoundBase* InteractSound; // 交互音效

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="FinalDoor",meta=(AllowPrivateAccess="true"))
	float ColorTolerance = 0.1f;

	UPROPERTY(EditAnywhere,Category = "FinalDoor",meta = (AllowPrivateAccess = "true"))
	float CameraTransitionTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "FinalDoor", meta = (AllowPrivateAccess = "true"))
	float CameraHoldTime = 2.0f;
	
	APawn* OriginalPawn = nullptr;
	FTimerHandle CameraTimerHandle;

	void ReturnToPlayerCamera();
};