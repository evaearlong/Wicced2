// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
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
    void PaintAtLocation(const FVector& CurrentWorldPos);

    UFUNCTION(BlueprintCallable)
    void ChangeBrushColor(FLinearColor NewColor);

    UFUNCTION(BlueprintCallable)
    void StartNewStroke();

protected:
    FVector PreviousWorldPos;
    bool bHasPrevious = false;

    UPROPERTY(EditAnywhere)
    float BrushSpacing = 0.005f;

    UPROPERTY(EditAnywhere)
    UNiagaraSystem* PaintSystem;

    UNiagaraComponent* NiagaraComp;

    UPROPERTY(EditAnywhere)
    float BrushSize = 32.0f;

    FLinearColor BrushColor = FLinearColor::Blue;

};
