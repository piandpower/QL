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
#include "QLGravityGunCompatibleActor.h"

// Sets default values
AQLGravityGunCompatibleActor::AQLGravityGunCompatibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(defaultBoxComponentDim));
    BoxComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
    BoxComponent->SetSimulatePhysics(true);
    BoxComponent->SetLinearDamping(defaultLinearDamping);
    BoxComponent->SetAngularDamping(defaultAngularDamping);
}

// Called when the game starts or when spawned
void AQLGravityGunCompatibleActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AQLGravityGunCompatibleActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}