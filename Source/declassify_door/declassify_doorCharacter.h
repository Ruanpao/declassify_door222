// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interface/InteractInterface.h"
#include "Component/InventoryComponent.h"

#include "declassify_doorCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class AStonePlate;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DELEGATE(FSwitchToStack1);
DECLARE_DELEGATE(FSwitchToStack2);
DECLARE_DELEGATE(FSwitchToStack3);
DECLARE_DELEGATE(FSwitchToStack4);
DECLARE_DELEGATE(FSwitchToStack5);
DECLARE_DELEGATE(FSwitchToStack6);
DECLARE_DELEGATE(FSwitchToStack7);
DECLARE_DELEGATE(FSwitchToStack8);
DECLARE_DELEGATE(FSwitchToStack9);
DECLARE_DELEGATE(FSwitchToStack10);


UCLASS(config=Game)
class Adeclassify_doorCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MouseClickAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack4Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack5Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack6Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack7Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack8Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack9Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SwitchToStack10Action;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* RotationAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PaintAction;
public:
	Adeclassify_doorCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void Interact();
	AActor* SphereTraceForInteractable();
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractRadius = 300.0f;

	UFUNCTION(BlueprintCallable, Category="MouseClick")
	void HandleMouseClick();

	void OnHoldedItemChanged(FItemInInventory Item);

	void SwitchToStack1();
	void SwitchToStack2();
	void SwitchToStack3();
	void SwitchToStack4();
	void SwitchToStack5();
	void SwitchToStack6();
	void SwitchToStack7();
	void SwitchToStack8();
	void SwitchToStack9();
	void SwitchToStack10();

	UPROPERTY(VisibleAnyWhere,BlueprintReadWrite,Category="Component")
	UInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintCallable, Category="Door")
	void RotateNearbyDoor();

	UFUNCTION(BlueprintCallable, Category="Door")
	void PaintDoor();

	//当前携带的油漆颜色
	UPROPERTY(BlueprintReadOnly, Category="Paint")
	FLinearColor CurrentPaintColor = FLinearColor::White;

	//是否携带油漆
	UPROPERTY(BlueprintReadOnly, Category="Paint")
	bool bHasPaint = false;

	//当前携带的石板类
	UPROPERTY(BlueprintReadOnly, Category="StonePlate")
	AStonePlate * CurrentStonePlate;

	//所有携带的石板
	UPROPERTY(BlueprintReadOnly, Category="StonePlate")
	TArray<FLinearColor>  CollectedPlateColors;

	//是否携带石板
	UPROPERTY(BlueprintReadOnly, Category="StonePlate")
	bool bHasStonePlate = false;

	UFUNCTION(BlueprintCallable, Category = "StonePlate")
	void PickupStonePlate(const FLinearColor NewPlateColor);

	UFUNCTION(BlueprintCallable,Category = "Paint")
	void PickupPaint(const FLinearColor& NewColor);
	
};

