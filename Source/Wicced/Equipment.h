// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Equipment.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	LeftHand	UMETA(DisplayName = "Left Hand"),
	RightHand   UMETA(DisplayName = "Right Hand"),
	BothHands   UMETA(DisplayName = "Both Hands")
};

UENUM(BlueprintType)
enum class EEquipmentAction : uint8
{
	Use        UMETA(DisplayName = "Use"),
	Attack     UMETA(DisplayName = "Attack"),
	None       UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FEquipmentUseResult
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	EEquipmentAction Action = EEquipmentAction::None;

	UPROPERTY(BlueprintReadOnly)
	float AimPitch = 0.f;

	FVector TargetHitLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	bool bHasAimTarget = false;
};

UCLASS(Blueprintable)
class WICCED_API AEquipment : public AActor
{
	GENERATED_BODY()

public:

	AEquipment();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* InteractZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UStaticMeshComponent* EquipmentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FGameplayTag EquipmentId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TMap<EEquipmentAction, UAnimMontage*> ActionAnimations;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FEquipmentUseResult UseEquipment(ACharacter* User);

protected:

	virtual void BeginPlay() override;

private:
	void SetupInteractZone();
};
