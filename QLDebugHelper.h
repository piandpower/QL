//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "GameFramework/Actor.h"
#include "QLDebugHelper.generated.h"

//------------------------------------------------------------
// usage:
// add to class declaration
// AQLDebugHelper* DebugHelper;
//
// add to BeginPlay()
// DebugHelper = GetWorld()->SpawnActor<AQLDebugHelper>(AQLDebugHelper::StaticClass());
// DebugHelper->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
// DebugHelper->SetActorRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
//
// add to Tick()
// DebugHelper->SetActorRotation(GetControlRotation());
//------------------------------------------------------------
UCLASS()
class QL_API AQLDebugHelper : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    AQLDebugHelper();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UStaticMeshComponent* XAxis;
    UStaticMeshComponent* YAxis;
    UStaticMeshComponent* ZAxis;
    UTextRenderComponent* XAxisLabel;
    UTextRenderComponent* YAxisLabel;
    UTextRenderComponent* ZAxisLabel;
    UBoxComponent* BoxComponent;
};
