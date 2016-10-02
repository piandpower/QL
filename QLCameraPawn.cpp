//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLCameraPawn.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLCameraPawn::AQLCameraPawn()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    QLCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    QLCameraComponent->SetupAttachment(RootComponent);
    QLCameraComponent->bUsePawnControlRotation = true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::BeginPlay()
{
    Super::BeginPlay();

    //DebugHelper = GetWorld()->SpawnActor<AQLDebugHelper>(AQLDebugHelper::StaticClass());
    //DebugHelper->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    //DebugHelper->SetActorRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
}