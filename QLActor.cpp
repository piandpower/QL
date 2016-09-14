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
#include "QLActor.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLActor::AQLActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    QLOwner = nullptr;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLActor::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::SetQLOwner(AActor* QLOwner)
{
    this->QLOwner = QLOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::UnSetQLOwner()
{
    this->QLOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AActor* AQLActor::GetQLOwner()
{
    return QLOwner;
}