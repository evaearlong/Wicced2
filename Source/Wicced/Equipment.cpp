// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "Components/BoxComponent.h"

// Sets default values
AEquipment::AEquipment()
{
    PrimaryActorTick.bCanEverTick = false;

    EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
    RootComponent = EquipmentMesh;
    EquipmentMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EquipmentMesh->SetSimulatePhysics(false);
    EquipmentMesh->SetEnableGravity(false);

    SetupInteractZone();
}

void AEquipment::SetupInteractZone() {

    InteractZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractZone"));
    InteractZone->SetupAttachment(RootComponent);

    InteractZone->SetBoxExtent(FVector(200.f, 200.f, 200.f));
    InteractZone->SetRelativeLocation(FVector::ZeroVector);
    InteractZone->SetSimulatePhysics(false);
    InteractZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
    Super::BeginPlay();
}

FEquipmentUseResult AEquipment::UseEquipment_Implementation(ACharacter* User)
{
    return FEquipmentUseResult();
}

