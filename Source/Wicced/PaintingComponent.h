// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PaintingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WICCED_API UPaintingComponent : public UActorComponent
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void FinishStroke();

    UFUNCTION(BlueprintCallable)
    void InitializePainting();

    UFUNCTION(BlueprintCallable)
    void PaintAtUV(const FVector2D& CurrentUV);

    UFUNCTION(BlueprintCallable)
    void ChangeBrushColor(FLinearColor NewColor);

protected:
    FVector2D PreviousUV;
    bool bHasPrevious = false;

    UPROPERTY(EditAnywhere)
    float BrushSpacing = 0.005f;

    UPROPERTY(EditAnywhere)
    float BrushSize = 32.0f;

    UPROPERTY()
    UTextureRenderTarget2D* RenderTarget;

    UPROPERTY(EditAnywhere)
    UTexture2D* BrushTexture;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* BrushMaterial;

    UPROPERTY(BlueprintReadWrite)
    UMaterialInstanceDynamic* BrushMID;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* CanvasMaterial;

    UPROPERTY(BlueprintReadWrite)
    UMaterialInstanceDynamic* CanvasMID;


};
