// Copyright Epic Games, Inc. All Rights Reserved.

#include "declassify_doorCharacter.h"
#include "declassify_doorProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Actor/RotateDoor.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// Adeclassify_doorCharacter

Adeclassify_doorCharacter::Adeclassify_doorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void Adeclassify_doorCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void Adeclassify_doorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Adeclassify_doorCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Adeclassify_doorCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &Adeclassify_doorCharacter::Interact);

		EnhancedInputComponent->BindAction(MouseClickAction, ETriggerEvent::Started, this, &Adeclassify_doorCharacter::HandleMouseClick);

		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Started, this, &Adeclassify_doorCharacter::RotateNearbyDoor);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

AActor* Adeclassify_doorCharacter::SphereTraceForInteractable()
{
	FVector StartLocation = GetActorLocation();
    
	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractRadius);
    
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		StartLocation, 
		FQuat::Identity,
		ECC_WorldDynamic, 
		Sphere,
		QueryParams
	);

	DrawDebugSphere(GetWorld(), StartLocation, InteractRadius, 12, FColor::Green, false, 2.0f);
    
	if (bHit)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
            
			if (HitActor && HitActor->Implements<UInteractInterface>())
			{
				DrawDebugPoint(GetWorld(), HitActor->GetActorLocation(), 20.0f, FColor::Red, false, 2.0f);
                
				UE_LOG(LogTemp, Log, TEXT("检测到可交互对象: %s"), *HitActor->GetName());
				return HitActor;
			}
		}
	}
    
	return nullptr;
}

void Adeclassify_doorCharacter::HandleMouseClick()
{
	if(!Controller)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(!PlayerController)
	{
		return;
	}

	//获得视口大小
	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float CenterX = ViewportSizeX / 2.0f;
	float CenterY = ViewportSizeY / 2.0f;

	//屏幕中心发射
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(CenterX, CenterY, WorldLocation, WorldDirection);

	FVector Start = WorldLocation;
	FVector End = Start + (WorldDirection * InteractRadius);
	
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Green,
		false,  
		0.5f,   
		0,      
		2.0f    
	);
	
	DrawDebugPoint(
		GetWorld(),
		Start,
		10.0f,
		FColor::Blue,
		false,
		0.5f
	);
	
	//射线检测
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_WorldDynamic,QueryParams);

	if(bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor)
		{
			UE_LOG(LogTemp, Display, TEXT("射线检测到物体: %s"), *HitActor->GetName()); 
			
			UFunction* ClickFunction = HitActor->FindFunction(FName("OnClick"));
			if(ClickFunction)
			{
				HitActor->ProcessEvent(ClickFunction, nullptr);
			}
			
		}
	}
}

void Adeclassify_doorCharacter::RotateNearbyDoor()
{
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARotateDoor::StaticClass(), FoundDoors);
	
	for (AActor* DoorActor : FoundDoors)
	{
		float Distance = FVector::Dist(GetActorLocation(), DoorActor->GetActorLocation());
		if (Distance < 500.0f)  
		{
			if (ARotateDoor* Door = Cast<ARotateDoor>(DoorActor))
			{
				Door->RotateDoor();
				return;  
			}
		}
	}
}

void Adeclassify_doorCharacter::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("尝试交互..."));
    
	AActor* InteractableActor = SphereTraceForInteractable();
    
	if (InteractableActor && InteractableActor->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_OnInteract(InteractableActor, this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("没有找到可交互的对象"));
	}
}


void Adeclassify_doorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void Adeclassify_doorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}