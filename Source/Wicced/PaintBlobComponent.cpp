// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintBlobComponent.h"

UPaintBlobComponent::UPaintBlobComponent() {

}

void UPaintBlobComponent::OnRegister()
{
    Super::OnRegister();

    if (PaintMaterial)
    {
        UMaterialInstanceDynamic* PaintMID = CreateDynamicMaterialInstance(0, PaintMaterial);

        if (PaintMID)
        {
            PaintMID->SetVectorParameterValue("PaintColor", PaintColor);
        }
    }
}