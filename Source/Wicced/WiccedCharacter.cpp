// Copyright Epic Games, Inc. All Rights Reserved.

#include "WiccedCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Wicced.h"

AWiccedCharacter::AWiccedCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AWiccedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWiccedCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AWiccedCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWiccedCharacter::Look);
	}
	else
	{
		UE_LOG(LogWicced, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AWiccedCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AWiccedCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AWiccedCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AWiccedCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AWiccedCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AWiccedCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AWiccedCharacter::EquipItem(AEquipment* NewItem, FName EquipmentSocketName)
{
	if (!NewItem) return;

	if (!EquippedItems.IsEmpty())
	{
		for (AEquipment* EquippedItem : EquippedItems) {
			if (EquippedItem->EquipmentType == NewItem->EquipmentType || NewItem->EquipmentType == EEquipmentType::BothHands) {
				UnequipItem(EquippedItem->EquipmentType);
			}
		}
	}

	AEquipment* EquippedItem = NewItem;
	EquippedItem->EquipmentMesh->SetSimulatePhysics(false);
	EquippedItem->EquipmentMesh->SetEnableGravity(false);
	EquippedItem->EquipmentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedItem->SetActorEnableCollision(false);
	EquippedItems.Add(EquippedItem);

	if (GetMesh())
	{
		EquippedItem->EquipmentMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipmentSocketName);
	}

}

FEquipmentUseResult AWiccedCharacter::UseEquippedItem(EEquipmentType Type)
{
	if (EquippedItems.IsEmpty())
	{
		return FEquipmentUseResult();
	}
	for (AEquipment* EquippedItem : EquippedItems) {
		if (EquippedItem->EquipmentType == Type)
		{
			FEquipmentUseResult Result = EquippedItem->UseEquipment(this);
			EquipmentAimPitch = Result.AimPitch;
			TargetLocation = Result.TargetHitLocation;
			bUsingEquipment = Result.bHasAimTarget;
			return Result;
		}
	}
	return FEquipmentUseResult();
}

void AWiccedCharacter::UnequipItem(EEquipmentType Type)
{
	if (EquippedItems.IsEmpty())
	{
		return;
	}

	for (AEquipment* EquippedItem : EquippedItems) {
		if (EquippedItem->EquipmentType == Type)
		{
			EquippedItems.Remove(EquippedItem);
			EquippedItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			EquippedItem->Destroy();
		}
	}
}
