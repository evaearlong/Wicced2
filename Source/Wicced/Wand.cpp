// Fill out your copyright notice in the Description page of Project Settings.
#include "Wand.h"


FEquipmentUseResult AWand::UseEquipment_Implementation(ACharacter* User)
{
    FEquipmentUseResult Result;

    if (!User) return Result;

    FVector Start = User->GetActorLocation();
    FVector Forward = User->GetActorForwardVector();
    FVector End = Start + Forward * 200.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(User);

    if (User->GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Visibility, Params))
    {
        if (Hit.GetActor())
        {
            FVector AimDir = (Hit.ImpactPoint - Start).GetSafeNormal();

            FVector LocalDir = User->GetActorTransform()
                .InverseTransformVectorNoScale(AimDir);

            float PitchRadians = FMath::Atan2(LocalDir.Z, LocalDir.X);
            float PitchDegrees = FMath::RadiansToDegrees(PitchRadians);

            constexpr float MaxPitchDegrees = 45.f;
            Result.AimPitch = FMath::Clamp(
                PitchDegrees / MaxPitchDegrees,
                -1.f,
                1.f
            );

            Result.bHasAimTarget = true;
            Result.TargetHitLocation = Hit.ImpactPoint;
            Result.Action = EEquipmentAction::Use;
            return Result;
        }

        Result.Action = EEquipmentAction::Attack;
        return Result;
    }

    Result.Action = EEquipmentAction::Attack;
    return Result;
}