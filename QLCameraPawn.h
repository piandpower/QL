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

#include "QLUtility.h"
#include "QLDebugHelper.h"
#include "GameFramework/Pawn.h"
#include "QLCameraPawn.generated.h"

UCLASS()
class QL_API AQLCameraPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLCameraPawn();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

private:
    UCameraComponent* QLCameraComponent;
    AQLDebugHelper* DebugHelper;
};
