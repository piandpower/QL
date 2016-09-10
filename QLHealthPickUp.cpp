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
#include "QLHealthPickUp.h"

// Sets default values
AQLHealthPickUp::AQLHealthPickUp()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(100.0f));
    BoxComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AQLHealthPickUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AQLHealthPickUp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}