// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "GameFramework/Character.h"
#include "Wand.generated.h"

/**
 * 
 */
UCLASS()
class WICCED_API AWand : public AEquipment
{

	GENERATED_BODY()
	virtual FEquipmentUseResult UseEquipment_Implementation(ACharacter* User);

};
