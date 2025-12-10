// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Components/SphereComponent.h" // 新增
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

	// 获取交互提示文本
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FText GetInteractText() const;

private:
	UPROPERTY(VisibleAnywhere,Category = "Paint")
	class UStaticMeshComponent* BucketMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PickupSound;
	// 新增：交互检测碰撞体
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	class USphereComponent* InteractionSphere;

	// 碰撞事件处理函数
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
