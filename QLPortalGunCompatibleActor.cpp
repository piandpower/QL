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
#include "QLWeaponPortalGun.h"

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

    TheOTherPortal = nullptr;
    PortalOwner = nullptr;

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
void AQLPortalGunCompatibleActor::SetPortalOwner(AQLWeaponPortalGun* PortalOwner)
{
    // set logical ownership
    // so that the portal will know which portal gun is owning it
    // and can call portal gun's member function
    this->PortalOwner = PortalOwner;
}

//------------------------------------------------------------
// The portal does not transport an actor when it
// is not character but is currently owned by character
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    FString info = FString("overlapping actor = ") + OtherActor->GetName();

    //// OtherActor is character
    //if (OtherActor->IsA(this->GetOwner()))
    //{
    //    info += " --> player.";
    //}
    //// OtherActor is not character
    //{
    //    // OtherActor's owner is character
    //    if (OtherActor->GetOwner() == PortalOwner->GetWeaponOwner())
    //    {
    //        info += " --> owned by player.";
    //    }
    //    else
    //    {
    //        info += " --> not owned by player.";
    //    }
    //}

    //QLUtility::QLSay(info);
}