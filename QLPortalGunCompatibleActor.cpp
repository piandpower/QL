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
#include "QLPortalGunCompatibleActor.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortalGunCompatibleActor::AQLPortalGunCompatibleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(100.0f));
    BoxComponent->SetSimulatePhysics(false);
    // should use ECollisionEnabled::QueryAndPhysics instead of ECollisionEnabled::QueryOnly
    // when though SetSimulatePhysics(false)
    // otherwise this actor will become transparent to gravity gun compatible actor
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    BoxComponent->SetWorldScale3D(FVector(1.0f, 0.2f, 1.0f));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
    StaticMeshComponent->SetStaticMesh(StaticMeshObj.Object);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -2.0f * zDim));
    StaticMeshComponent->SetWorldScale3D(FVector(2.0f, 0.4f, 2.0f));
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}