// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Canvas.h"

void UPaintingComponent::FinishStroke() {
    bHasPrevious = false;
}

void UPaintingComponent::InitializePainting()
{
    UWorld* World = GetWorld();
    if (!World) return;

    RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
        World,
        1024,  
        1024,  
        RTF_RGBA8
    );

    UKismetRenderingLibrary::ClearRenderTarget2D(
        World,
        RenderTarget,
        FLinearColor::White 
    );

    if (BrushMaterial)
    {
        BrushMID = UMaterialInstanceDynamic::Create(BrushMaterial, this);
        BrushMID->SetVectorParameterValue("BrushColor", FLinearColor::Black);
        BrushMID->SetScalarParameterValue("BrushSize", BrushSize);
        BrushMID->SetTextureParameterValue("BrushTexture", BrushTexture);
    }

    if (CanvasMaterial)
    {
        CanvasMID = UMaterialInstanceDynamic::Create(CanvasMaterial, this);
        CanvasMID->SetTextureParameterValue("RenderTarget", RenderTarget);
    }

}

void UPaintingComponent::ChangeBrushColor(FLinearColor NewColor) {
    if (BrushMID) {
        BrushMID->SetVectorParameterValue("BrushColor", NewColor);
    }
}

void UPaintingComponent::PaintAtUV(const FVector2D& CurrentUV)
{
    if (!RenderTarget || !BrushMaterial) return;

    UWorld* World = GetWorld();
    if (!World) return;

    if (!bHasPrevious)
    {
        PreviousUV = CurrentUV;
        bHasPrevious = true;
    }

    float Distance = FVector2D::Distance(PreviousUV, CurrentUV);

    int32 Steps = FMath::Max(1, FMath::FloorToInt(Distance / BrushSpacing));

    FDrawToRenderTargetContext Context;
    UCanvas* Canvas;
    FVector2D Size;

    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(
        World,
        RenderTarget,
        Canvas,
        Size,
        Context
    );

    for (int32 i = 0; i <= Steps; i++)
    {
        float Alpha = (Steps == 0) ? 0.f : (float)i / (float)Steps;

        FVector2D UV = FMath::Lerp(PreviousUV, CurrentUV, Alpha);

        FVector2D PixelPos = FVector2D(UV.X * Size.X, UV.Y * Size.Y);

        float DrawSize = BrushSize;

        FVector2D DrawPos = PixelPos - FVector2D(DrawSize * 0.5f, DrawSize * 0.5f);

        Canvas->K2_DrawMaterial(
            BrushMID,
            DrawPos,
            FVector2D(DrawSize, DrawSize),
            FVector2D(0, 0),
            FVector2D(1, 1),
            0.0f
        );
    }

    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(World, Context);

    PreviousUV = CurrentUV;
}
