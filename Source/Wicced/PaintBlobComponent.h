// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PaintBlobComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WICCED_API UPaintBlobComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UPaintBlobComponent();

	virtual void OnRegister() override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* PaintMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor PaintColor = FLinearColor::Black;
};
