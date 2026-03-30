// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Canvas.h"

void UPaintingComponent::FinishStroke() {
    bHasPrevious = false;
}

void UPaintingComponent::ChangeBrushColor(FLinearColor NewColor) {
    BrushColor = NewColor;

    if (NiagaraComp)
    {
        NiagaraComp->SetVariableLinearColor(TEXT("User.BrushColor"), NewColor);
    }
}

void UPaintingComponent::StartNewStroke()
{
    UWorld* World = GetWorld();
    if (!World || !PaintSystem) return;

    NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
        PaintSystem,
        GetOwner()->GetRootComponent(),
        NAME_None,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        true
    );
    NiagaraComp->SetVariableLinearColor(TEXT("User.BrushColor"), BrushColor);
    NiagaraComp->SetVariableFloat(TEXT("User.BrushSize"), BrushSize);

    bHasPrevious = false;
}

void UPaintingComponent::PaintAtLocation(const FVector& CurrentWorldPos)
{
    if (!NiagaraComp) return;

    if (bHasPrevious)
    {
        float Dist = FVector::Distance(PreviousWorldPos, CurrentWorldPos);
        if (Dist < BrushSpacing * 0.5f) return;
    }

    NiagaraComp->SetVariableVec3(TEXT("User.PaintPosition"), CurrentWorldPos);

    float SizeVariation = FMath::FRandRange(0.8f, 1.2f);
    NiagaraComp->SetVariableFloat(TEXT("User.BrushSize"), BrushSize * SizeVariation);

    PreviousWorldPos = CurrentWorldPos;
    bHasPrevious = true;
}