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
#include "QLPortal.h"
#include "QLCharacter.h"
#include "QLWeaponPortalGun.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortal::AQLPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(100.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
    StaticMeshComponent->SetStaticMesh(StaticMeshObj.Object);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    StaticMeshComponent->SetWorldScale3D(FVector(2.0f));
    float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -zDim));

    TheOTherPortal = nullptr;
    PortalOwner = nullptr;

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLPortal::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortal::BeginPlay()
{
	Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortal::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetPortalOwner(AQLWeaponPortalGun* PortalOwner)
{
    // set logical ownership
    // so that the portal will know which portal gun is owning it
    // and can call portal gun's member function
    this->PortalOwner = PortalOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponPortalGun* AQLPortal::GetPortalOwner()
{
    return PortalOwner;
}

//------------------------------------------------------------
// --- note that when a portal happens to overlap another actor when
//     spawned, the overlap event seems to be trigger inside of SpawnActor
//     before the portal owner is manually set, which means at that time
//     PortalOwner is still nullptr !!!
//     To guarantee overlap is called after the actor is spawned, use deferred
//     actor spawn method. See AQLWeaponPortalGun::CreatePortal()
// --- The portal does not transport an actor if it
//     is not character but is currently owned by character
// --- If an actor has 2 components, each supports overlap event,
//     upon creation by SpawnActor(), the 2 components will trigger
//     their overlap event against each other, resulting in 2
//     overlap call, in name of the actor itself. So here we always
//     specify physics/collision in RootComponent and set the children
//     components to NoCollision and Ignore.
//------------------------------------------------------------
void AQLPortal::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    QLUtility::QLSay(FString("portal say: transport actor = ") + OtherActor->GetName());

    // if OtherActor is character
    // this is not a type check, but identity check
    //if (OtherActor == PortalOwner->GetWeaponOwner())
    //{
    //    info += " --> player.";
    //}

    //FVector DestLocation = OtherActor->GetActorLocation() + FVector(1000.0f, 1000.0f, 0.0f);
    //OtherActor->TeleportTo(DestLocation, FRotator::ZeroRotator);
}