// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "HidePass.generated.h"

UCLASS()
class DECLASSIFY_DOOR_API AHidePass : public AActor
{
	GENERATED_BODY()
	
public:	
	AHidePass();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="Pass")
	void SetRequiredColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable,Category="Pass")
	void CheckAndReveal(const FLinearColor& DoorColor);

	UFUNCTION(BlueprintCallable,Category="Pass")
	void SetWallVisible(bool bVisible);

	UFUNCTION(BlueprintCallable,Category="Pass")
	bool IsRevealed() const{return bIsRevealed;}

	UFUNCTION(BlueprintCallable,Category="Pass")
	UTextRenderComponent* GetTextRenderComponent() const{return TextRenderer;}

private:
	UPROPERTY(VisibleAnywhere,Category="Pass")
	class UStaticMeshComponent* WallMesh;

	UPROPERTY(VisibleAnywhere,Category="Pass")
	class UTextRenderComponent* TextRenderer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pass",meta=(AllowPrivateAccess="true"))
	FLinearColor RequiredColor = FLinearColor::Red;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Pass",meta=(AllowPrivateAccess="true"))
	bool bIsRevealed = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pass",meta=(AllowPrivateAccess="true"))
	float ColorTolerance = 0.1f;
};
