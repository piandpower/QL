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
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortalGunCompatibleActor::AQLPortalGunCompatibleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(200.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLPortalGunCompatibleActor::OnOverlapBeginForActor);
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

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    FString info = FString("overlapping actor = ") + OtherActor->GetName();
    AQLCharacter* player = Cast<AQLCharacter>(OtherActor);
    if (player)
    {
        info += " --> player.";
    }

    QLUtility::QLSay(info);
}